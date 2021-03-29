LOCAL_PATH := $(call my-dir)
LIBUSB_ROOT_REL := $(LOCAL_PATH)
LIBUSB_ROOT_ABS := $(LOCAL_PATH)

# libusb

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
  $(LIBUSB_ROOT_REL)/libusb/core.c \
  $(LIBUSB_ROOT_REL)/libusb/descriptor.c \
  $(LIBUSB_ROOT_REL)/libusb/hotplug.c \
  $(LIBUSB_ROOT_REL)/libusb/io.c \
  $(LIBUSB_ROOT_REL)/libusb/sync.c \
  $(LIBUSB_ROOT_REL)/libusb/strerror.c \
  $(LIBUSB_ROOT_REL)/libusb/os/linux_usbfs.c \
  $(LIBUSB_ROOT_REL)/libusb/os/events_posix.c \
  $(LIBUSB_ROOT_REL)/libusb/os/threads_posix.c \
  $(LIBUSB_ROOT_REL)/libusb/os/linux_netlink.c \
  $(LIBUSB_ROOT_REL)/libglsusb.cpp

LOCAL_C_INCLUDES += \
  $(LOCAL_PATH)/.. \
  $(LIBUSB_ROOT_ABS)/libusb \
  $(LIBUSB_ROOT_ABS)/libusb/os

LOCAL_EXPORT_C_INCLUDES := \
  $(LIBUSB_ROOT_ABS)/libusb

LOCAL_CFLAGS := -fvisibility=hidden -pthread

LOCAL_LDLIBS := -llog

LOCAL_MODULE := glsusb

include $(BUILD_SHARED_LIBRARY)
