#!/bin/sh

#working in Galaxy S20
#adb push obj/local/arm64-v8a/hello /data/local/tmp
#adb push obj/local/armeabi-v7a/hello /data/local/tmp

#not working in Galaxy S20
#adb push obj/local/x86/hello /data/local/tmp
#adb push obj/local/x86_64/hello /data/local/tmp

adb push obj/local/armeabi-v7a/hello /data/local/tmp
adb shell "chmod 777 /data/local/tmp/hello"
