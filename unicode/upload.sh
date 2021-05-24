#!/bin/sh

adb push obj/local/armeabi-v7a/unicode /data/local/tmp
adb shell "chmod 777 /data/local/tmp/unicode"
