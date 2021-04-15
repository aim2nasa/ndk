#include <jni.h>
#include <string>
#include <android/log.h>
#include <errno.h>
#define TAG "gls"

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_hellocpp_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {

    char name[128];
    memset(name,0,sizeof(name));

    //sprintf(name,"/sdcard/test1.dat");            //Operation not permitted
    //sprintf(name,"/data/local/tmp/test.dat");     //Permission denied
    //sprintf(name,"/storage/emulated/0/GlsZing/tmp.gls");    //No such file or directory
    //sprintf(name,"/sdcard/a.txt");          //Operation not permitted
    //sprintf(name,"/sdcard/a.txt");          //Operation not permitted
    //sprintf(name,"/sdcard/download/a.txt");          //ok
    //sprintf(name,"/sdcard/a.txt");                 //Operation not permitted
    //sprintf(name,"/sdcard/download/a.dat");       //ok

    sprintf(name,"/sdcard/download/a.dat");
    FILE *pFile = fopen( name, "w" );
    std::string str;
    if(pFile) {
        size_t szWrite = fwrite(name,1,sizeof(name),pFile);
        str = "fopen ok : " + std::to_string(szWrite);
        __android_log_print(ANDROID_LOG_INFO, TAG, "fopen ok, written bytes=%zu",szWrite);
        fclose(pFile);
    }else{
        str = "fopen failed : " + std::string(strerror(errno));
        __android_log_print(ANDROID_LOG_INFO, TAG, "fopen failed, %s",strerror(errno));
    }
    return env->NewStringUTF(str.c_str());
}