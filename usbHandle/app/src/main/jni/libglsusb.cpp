//
// Created by rossi on 21. 3. 23.
//

#include "com_example_usbhandle_MainActivity.h"
#include <libusb.h>

JNIEXPORT jint JNICALL Java_com_example_usbhandle_MainActivity_helloNNDK
  (JNIEnv *, jobject, jint v)
  {
    return v+10;
  }

JNIEXPORT jint JNICALL Java_com_example_usbhandle_MainActivity_open
        (JNIEnv *, jobject)
{
    return libusb_init(NULL);
}

JNIEXPORT void JNICALL Java_com_example_usbhandle_MainActivity_close
        (JNIEnv *, jobject)
{

}
