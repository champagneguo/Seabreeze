/***************************************************//**
 * @file    EEPROMSlotFeatureBase.cpp
 * @date    February 2009
 * @author  Ocean Optics, Inc.
 *
 * LICENSE:
 *
 * SeaBreeze Copyright (C) 2014, Ocean Optics Inc
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *******************************************************/

#include "common/globals.h"
#include "vendors/OceanOptics/features/eeprom_slots/EEPROMSlotFeatureBase.h"
#include "vendors/OceanOptics/protocols/interfaces/EEPROMProtocolInterface.h"
#include "vendors/OceanOptics/protocols/ooi/impls/OOIEEPROMProtocol.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "common/exceptions/FeatureProtocolNotFoundException.h"
#include "common/exceptions/FeatureControlException.h"

#ifndef _WINDOWS
#include <xlocale.h>
#endif

using namespace seabreeze;
using namespace seabreeze::ooiProtocol;
using namespace std;

#ifdef _WINDOWS
#pragma warning (disable: 4101) // unreferenced local variable
#endif

EEPROMSlotFeatureBase::EEPROMSlotFeatureBase() {
    this->protocols.push_back(new OOIEEPROMProtocol());
}

EEPROMSlotFeatureBase::~EEPROMSlotFeatureBase() {

}

vector<byte> *EEPROMSlotFeatureBase::readEEPROMSlot(const Protocol &protocol,
        const Bus &bus, unsigned int slot) throw (FeatureException) {

    EEPROMProtocolInterface *eeprom = NULL;
    ProtocolHelper *proto;

    try {
        proto = lookupProtocolImpl(protocol);
        eeprom = static_cast<EEPROMProtocolInterface *>(proto);
    } catch (FeatureProtocolNotFoundException &fpnfe) {
        string error(
                "Could not find matching protocol implementation to get read EEPROM.");
        /* FIXME: previous exception should probably be bundled up into the new exception */
        throw FeatureProtocolNotFoundException(error);
    }

    vector<byte> *info = NULL;

    try {
        info = eeprom->readEEPROMSlot(bus, slot);
    } catch (ProtocolException &pe) {
        string error("Caught protocol exception: ");
        error += pe.what();
        /* FIXME: previous exception should probably be bundled up into the new exception */
        throw FeatureControlException(error);
    }

    if(NULL == info) {
        /* FIXME:
         * If we can guarantee that all protocol implementations will either
         * return a valid value or throw an exception, this test can be
         * removed.
         */
        string error("Could not read EEPROM slot.");
        throw FeatureControlException(error);
    }

    return info;
}

int EEPROMSlotFeatureBase::writeEEPROMSlot(const Protocol &protocol,
        const Bus &bus, unsigned int slot, const vector<byte> &data) throw (FeatureException) {

    int bytesWritten = 0;

    EEPROMProtocolInterface *eeprom = NULL;
    ProtocolHelper *proto;

    try {
        proto = lookupProtocolImpl(protocol);
        eeprom = static_cast<EEPROMProtocolInterface *>(proto);
    } catch (FeatureProtocolNotFoundException &fpnfe) {
        string error("Could not find matching protocol implementation to get write EEPROM.");
        throw FeatureProtocolNotFoundException(error);
    }

    try {
        bytesWritten = eeprom->writeEEPROMSlot(bus, slot, data);
    } catch (ProtocolException &pe) {
        throw FeatureControlException(string("Caught protocol exception: ") + pe.what());
    }

    return bytesWritten;
}

double EEPROMSlotFeatureBase::readDouble(const Protocol &protocol, const Bus &bus,
        unsigned int slotNumber) throw (FeatureException, NumberFormatException) {

    char buffer[20];
    char *startPtr = NULL;
    char *endPtr = NULL;

    double retval = 0.0;

    /* This may throw a FeatureException, but cannot return NULL. */
    vector<byte> *slot = readEEPROMSlot(protocol, bus, slotNumber);

    /* Convert the ASCII string in the slot to a double in a way where we can
     * catch format/parse errors.
     */

    /* First, guarantee that the string we parse is null-terminated. 20 bytes is overkill. */
    strncpy(buffer, ((char *)&((*slot)[0])), 19);
    buffer[19] = '\0';
    startPtr = buffer;
    endPtr = NULL;
    errno = 0;

    /* Now parse the slot. */
#ifdef _WINDOWS
    retval = strtod(startPtr, &endPtr);
#else
//    retval = strtod(startPtr, &endPtr, LC_GLOBAL_LOCALE);
      retval = strtod(startPtr, &endPtr);
#endif
    if((startPtr == endPtr) || ((errno != 0) && (0 == retval))) {
        /* This means that strtod_l failed to parse anything, so the EEPROM slot
         * may have been unprogrammed or otherwise corrupted.  Flag an error
         * so that we can drop in some safe default values.
         */
        string error("Could not parse double out of EEPROM slot.");
        delete slot;

        throw NumberFormatException(error);
    }

    delete slot;

    return retval;
}


long EEPROMSlotFeatureBase::readLong(const Protocol &protocol, const Bus &bus,
            unsigned int slotNumber) throw (FeatureException, NumberFormatException) {
    char buffer[20];
    char *startPtr = NULL;
    char *endPtr = NULL;

    long retval = 0;

    /* This may throw a FeatureException, but cannot return NULL. */
    vector<byte> *slot = readEEPROMSlot(protocol, bus, slotNumber);

    /* Convert the ASCII string in the slot to a long in a way where we can
     * catch format/parse errors.
     */

    /* First, guarantee that the string we parse is null-terminated. 20 bytes is overkill. */
    strncpy(buffer, ((char *)&((*slot)[0])), 19);
    buffer[19] = '\0';
    startPtr = buffer;
    endPtr = NULL;
    errno = 0;

    /* Now parse the slot. */
#ifdef _WINDOWS
    retval = strtol(startPtr, &endPtr, 10);
#else
    //retval = strtol_l(startPtr, &endPtr, 10, LC_GLOBAL_LOCALE);
    retval = strtol(startPtr, &endPtr, 10);
#endif
    if((startPtr == endPtr) || ((errno != 0) && (0 == retval))) {
        /* This means that strtol_l failed to parse anything, so the EEPROM slot
         * may have been unprogrammed or otherwise corrupted.  Flag an error
         * so that we can drop in some safe default values.
         */
        string error("Could not parse int out of EEPROM slot.");
        delete slot;

        throw NumberFormatException(error);
    }

    delete slot;

    return retval;
}
