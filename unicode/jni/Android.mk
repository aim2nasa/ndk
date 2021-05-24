LOCAL_PATH:=$(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE:= unicode 
LOCAL_SRC_FILES:= unicode.cpp

include $(BUILD_EXECUTABLE)
