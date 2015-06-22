LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE:= static

LOCAL_SRC_FILES := libCombine.a 
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := api_test
LOCAL_SRC_FILES := api_test.c
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_STATIC_LIBRARIES := static
LOCAL_LDLIBS := -llog

#打印出变量值
#$(warning $(LOCAL_SRC_FILES))
#$(warning $(LOCAL_C_INCLUDES))

LOCAL_LDFLAGS += -L$(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/4.8/libs/armeabi/ -lgnustl_static -lsupc++
#LOCAL_LDFLAGS += -shared

LOCAL_CPPFLAGS += -frtti
LOCAL_CPPFLAGS += -fexceptions



include $(BUILD_EXECUTABLE)


