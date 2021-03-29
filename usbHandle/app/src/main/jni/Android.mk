LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libusb1.0
LOCAL_SRC_FILES := /home/rossi/libusb/android/libs/arm64-v8a/libusb1.0.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := testLib
LOCAL_SRC_FILES := test.cpp libcyusb.cpp
LOCAL_C_INCLUDES += /home/rossi/libusb/libusb
LOCAL_SHARED_LIBRARIES := libusb1.0

include $(BUILD_SHARED_LIBRARY)
