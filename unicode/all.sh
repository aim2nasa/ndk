#!/bin/sh

ndk-build clean
ndk-build

adb shell "rm -r /data/local/tmp/unicode"
adb push obj/local/arm64-v8a/unicode /data/local/tmp
adb shell "chmod 777 /data/local/tmp/unicode"
adb shell "/data/local/tmp/unicode"
