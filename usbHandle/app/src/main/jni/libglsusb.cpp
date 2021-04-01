//
// Created by rossi on 21. 3. 23.
//

#include "com_example_usbhandle_MainActivity.h"
#include <jni/libusb/libusbi.h>
#include <android/log.h>

#define TAG "glsusb"

static libusb_device_handle *devh = NULL;

JNIEXPORT jint JNICALL Java_com_example_usbhandle_MainActivity_helloNNDK
  (JNIEnv *, jobject, jint v)
  {
    return v+10;
  }

JNIEXPORT jint JNICALL Java_com_example_usbhandle_MainActivity_open
        (JNIEnv *, jobject, jint fileDescriptor)
{
    int r;

    r = libusb_set_option(NULL, LIBUSB_OPTION_WEAK_AUTHORITY, NULL);
    if(r<0) return r;

    r = libusb_init(NULL);
    if(r<0) return r;

    return libusb_wrap_sys_device(NULL,(intptr_t)fileDescriptor,&devh);
}

JNIEXPORT void JNICALL Java_com_example_usbhandle_MainActivity_close
        (JNIEnv *, jobject)
{
    libusb_exit(NULL);
}

int deviceInfo(libusb_device_handle *h)
{
    libusb_device *dev = libusb_get_device(h);
    struct libusb_device_descriptor desc;
    unsigned char string[256];

    int r = libusb_get_device_descriptor(dev,&desc);
    __android_log_print(ANDROID_LOG_INFO,TAG,"libusb_get_device_descriptor = %d",r);
    if(r<0) return r;

    if(desc.iManufacturer) {
        if(libusb_get_string_descriptor_ascii(devh,desc.iManufacturer,string,sizeof(string))>0)
            __android_log_print(ANDROID_LOG_INFO,TAG,"Manufacturer: %s",(char*)string);
    }

    if(desc.iProduct) {
        if(libusb_get_string_descriptor_ascii(devh,desc.iProduct,string,sizeof(string))>0)
            __android_log_print(ANDROID_LOG_INFO,TAG,"Product: %s",(char*)string);
    }

    if(desc.iSerialNumber) {
        if(libusb_get_string_descriptor_ascii(devh,desc.iSerialNumber,string,sizeof(string))>0)
            __android_log_print(ANDROID_LOG_INFO,TAG,"Serial Number: %s",(char*)string);
    }
    return 0;
}

JNIEXPORT jint JNICALL Java_com_example_usbhandle_MainActivity_reader
        (JNIEnv *, jobject)
{
    __android_log_print(ANDROID_LOG_INFO,TAG,"reader starts");
    if(!devh) {
        __android_log_print(ANDROID_LOG_ERROR,TAG,"device handle is null");
        return -101;
    }

    int r = deviceInfo(devh);
    __android_log_print(ANDROID_LOG_INFO,TAG,"deviceInfo = %d",r);
    if(r<0) return r;
    
    return 0;
}