LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := usbtest
LOCAL_SRC_FILES := testlibusb.c
LOCAL_C_INCLUDES := $(LOCAL_PATH)
LOCAL_SHARED_LIBRARY := libusb
include $(BUILD_EXECUTABLE)
$(call import-module,libusb)
