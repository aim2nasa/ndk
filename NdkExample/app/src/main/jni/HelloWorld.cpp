//
// Created by rossi on 2021-03-22.
//
#include "com_example_ndkexample_MainActivity.h"

JNIEXPORT jint JNICALL Java_com_example_ndkexample_MainActivity_helloNDK
  (JNIEnv *, jobject, jint v){
    return v+10;
  }
