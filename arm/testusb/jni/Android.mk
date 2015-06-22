LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE:= static
LOCAL_SRC_FILES := libusb.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := find_mice
LOCAL_SRC_FILES := find_mice.cpp
LOCAL_C_INCLUDES := $(LOCAL_PATH)
LOCAL_STATIC_LIBRARIES := static
LOCAL_LDLIBS := -llog

include $(BUILD_EXECUTABLE)


