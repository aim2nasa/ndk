//
// Created by rossi on 21. 3. 23.
//

#include "com_example_usbhandle_MainActivity.h"
#include <jni/libusb/libusbi.h>

JNIEXPORT jint JNICALL Java_com_example_usbhandle_MainActivity_helloNNDK
  (JNIEnv *, jobject, jint v)
  {
    return v+10;
  }

JNIEXPORT jint JNICALL Java_com_example_usbhandle_MainActivity_open
        (JNIEnv *, jobject, jint fileDescriptor)
{
    int r;
    libusb_context ctx;
    libusb_context *pctx = &ctx;
    libusb_device_handle *devh;

    r = libusb_set_option(&ctx, LIBUSB_OPTION_WEAK_AUTHORITY, NULL);
    if(r<0) return r;

    r = libusb_init(&pctx);
    if(r<0) return r;

    r = libusb_wrap_sys_device(NULL,(intptr_t)fileDescriptor,&devh);
    return r;
}

JNIEXPORT void JNICALL Java_com_example_usbhandle_MainActivity_close
        (JNIEnv *, jobject)
{

}
