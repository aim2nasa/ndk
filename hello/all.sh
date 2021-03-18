#!/bin/sh

ndk-build clean
ndk-build

adb shell "rm -r /data/local/tmp/hello"
adb push obj/local/armeabi-v7a/hello /data/local/tmp
adb shell "chmod 777 /data/local/tmp/hello"
adb shell "/data/local/tmp/hello"
