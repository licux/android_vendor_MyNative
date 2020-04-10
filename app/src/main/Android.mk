
LOCAL_PATH := $(call my-dir)

$(warning mynative)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    cpp/drawer.cpp \
    cpp/main.cpp \

LOCAL_MODULE := libmynative
LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv2 
LOCAL_WHOLE_STATIC_LIBRARIES := libandroid_native_app_glue

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_SDK_VERSION := current
LOCAL_MIN_SDK_VERSION := 21
LOCAL_PACKAGE_NAME := MyNative
LOCAL_JAVA_LIBRARIES := framework

include $(BUILD_PACKAGE)
