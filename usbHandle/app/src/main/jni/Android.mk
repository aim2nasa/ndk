LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := testLib
LOCAL_SRC_FILES := test.cpp
LOCAL_C_INCLUDES += /home/rossi/libusb/libusb

include $(BUILD_SHARED_LIBRARY)