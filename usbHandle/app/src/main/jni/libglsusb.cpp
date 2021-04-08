//
// Created by rossi on 21. 3. 23.
//

#include "com_example_usbhandle_MainActivity.h"
#include <jni/libusb/libusbi.h>
#include <android/log.h>
#include <string.h>

#define TAG "glsusb"
#define BUF_SIZE    (8192*8)

static libusb_device_handle *devh = NULL;
static unsigned int count;
unsigned char epi = 0x82;   //Input EP
unsigned char epo = 0x02;   //Output EP

JNIEXPORT jint JNICALL Java_com_example_usbhandle_MainActivity_helloNNDK
  (JNIEnv *, jobject, jint v)
{
    __android_log_print(ANDROID_LOG_INFO,TAG,"helloNDK");
    return v+20;
}

static int deviceInfo(libusb_device_handle *h)
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

JNIEXPORT jint JNICALL Java_com_example_usbhandle_MainActivity_open
        (JNIEnv *, jobject, jint fileDescriptor)
{
    __android_log_print(ANDROID_LOG_INFO,TAG,"open starts");
    int r;

    r = libusb_set_option(NULL, LIBUSB_OPTION_WEAK_AUTHORITY, NULL);
    if(r<0) {
        __android_log_print(ANDROID_LOG_INFO,TAG,"libusb_set_option error=%d",r);
        return r;
    }

    r = libusb_init(NULL);
    if(r<0) {
        __android_log_print(ANDROID_LOG_INFO,TAG,"libusb_init error=%d",r);
        return r;
    }

    r = libusb_wrap_sys_device(NULL,(intptr_t)fileDescriptor,&devh);
    if(r<0) {
        __android_log_print(ANDROID_LOG_INFO,TAG,"libusb_wrap_sys_device error=%d",r);
        return r;
    }

    r = deviceInfo(devh);
    __android_log_print(ANDROID_LOG_INFO,TAG,"deviceInfo = %d",r);
    if(r<0) return r;

    r = libusb_kernel_driver_active(devh,0);
    __android_log_print(ANDROID_LOG_INFO,TAG,"libusb_kernel_driver_active = %d",r);
    if(r<0) return r;

    return 0;
}

JNIEXPORT void JNICALL Java_com_example_usbhandle_MainActivity_close
        (JNIEnv *, jobject)
{
    libusb_exit(NULL);
}

static void* runThread(void *arg)
{
    int r;
    int transferred = 0;
    unsigned char ep = *((unsigned char*)arg);

    unsigned char *buf = new unsigned char[BUF_SIZE];

    libusb_clear_halt(devh,ep);
    __android_log_print(ANDROID_LOG_INFO,TAG,"runThread starts(ep:0x%x)...",ep);
    memset(buf,'\0',BUF_SIZE);
    count = 0;
    while(1){
        r = libusb_bulk_transfer(devh,ep,buf,sizeof(unsigned char)*BUF_SIZE,&transferred,0);
        if(r==0){
            __android_log_print(ANDROID_LOG_INFO,TAG,"%u %dbytes",++count,transferred);
        }else{
            delete [] buf;
            __android_log_print(ANDROID_LOG_ERROR,TAG,"libusb_bulk_transfer=%d",r);
            return NULL;
        }
    }
}

JNIEXPORT jint JNICALL Java_com_example_usbhandle_MainActivity_reader
        (JNIEnv *, jobject)
{
    __android_log_print(ANDROID_LOG_INFO,TAG,"reader starts");
    pthread_t tid;
    return pthread_create(&tid,NULL,runThread,&epi);
}

JNIEXPORT jlong JNICALL Java_com_example_usbhandle_MainActivity_count
        (JNIEnv *, jobject)
{
    return count;
}

JNIEXPORT jint JNICALL Java_com_example_usbhandle_MainActivity_writer
        (JNIEnv *, jobject)
{
    __android_log_print(ANDROID_LOG_INFO,TAG,"writer starts");
    pthread_t tid;
    return pthread_create(&tid,NULL,runThread,&epo);
}