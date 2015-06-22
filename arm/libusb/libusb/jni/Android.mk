LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := usb
LOCAL_SRC_FILES := linux.c \ error.c \ usb.c \ descriptors.c \ usbpp.cpp
LOCAL_C_INCLUDES := $(LOCAL_PATH)

include $(BUILD_STATIC_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE    := testusb
LOCAL_SRC_FILES := testlibusb.c
LOCAL_STATIC_LIBRARIES := usb

include $(BUILD_EXECUTABLE)
                    
