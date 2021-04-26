#include <jni.h>
#include <string>
#include <android/log.h>
#include <unistd.h>
#include <pthread.h>

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_nativethreadcallback_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

JavaVM * glpVM = NULL;
int end_flag;
jclass jObject = NULL;
jmethodID funccb = NULL;

void Notify(int n) {
    int value = 0 ;
    value = n ;
    if ( !glpVM ) {
        __android_log_print( ANDROID_LOG_INFO, "NTC", "error (!glpVM)" ) ;
        return ;
    }

    if ( !funccb ) {
        __android_log_print( ANDROID_LOG_INFO, "NTC", "error (!funccb)" ) ;
        return ;
    }

    JNIEnv* env = NULL ;
    glpVM->AttachCurrentThread(  &env, NULL ) ;
    if ( env == NULL || jObject == NULL ) {
        glpVM->DetachCurrentThread() ;
        __android_log_print( ANDROID_LOG_INFO, "NTC", "error (env == NULL || AVM_JM.JObject == NULL)" ) ;
        return ;
    }

    env->CallStaticVoidMethod(  jObject, funccb, value ) ;
    glpVM->DetachCurrentThread( ) ;
}

void *t_function(void *data)
{
    int id;
    int i=0;
    id = *((int *)data);

    while(end_flag)
    {
        Notify(i);
        i++;
        sleep(1);
    }
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_example_nativethreadcallback_MainActivity_startThread(JNIEnv *env, jobject thiz) {
    end_flag = 1;
    int b = 2 ;

    __android_log_print( ANDROID_LOG_INFO, "NTC", "Call start thread" ) ;
    jclass cls ;
    cls = env->FindClass(  "com/example/nativethreadcallback/MainActivity") ;
    if ( cls == NULL ) {
        __android_log_print( ANDROID_LOG_INFO, "NTC", "Can't find the class.") ;
    }

    jObject = (jclass)env->NewGlobalRef( cls ) ;
    funccb = env->GetStaticMethodID( cls, "callback", "(I)V" ) ;
    if ( funccb == 0 ) {
        __android_log_print( ANDROID_LOG_INFO, "NTC", "Can't find the function." ) ;
        env->DeleteGlobalRef( jObject ) ;
    }
    else {
        __android_log_print( ANDROID_LOG_INFO, "NTC", "Method connect success....\n") ;
        env->CallStaticVoidMethod( cls, funccb, 10 ) ;
    }

    pthread_t p_thread;
    int thr_id = pthread_create(&p_thread, NULL, t_function, (void *) &b);
    if ( thr_id < 0 ) {
        __android_log_print( ANDROID_LOG_INFO, "NTC", "Create thread fail.\n" ) ;
        return -1 ;
    }
    return 0 ;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_example_nativethreadcallback_MainActivity_endThread(JNIEnv *env, jobject thiz) {
    __android_log_print( ANDROID_LOG_INFO, "NTC", "Call end thread" ) ;
    end_flag = 0 ;
    return 0 ;
}

static JNINativeMethod methods[] = {
        {"startThread", "()I", (void*)Java_com_example_nativethreadcallback_MainActivity_startThread },
        {"endThread", "()I", (void*)Java_com_example_nativethreadcallback_MainActivity_endThread },
};

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    jint result = -1 ;
    JNIEnv* env = NULL ;
    jclass cls ;

    if ( vm->GetEnv(  (void**)&env, JNI_VERSION_1_4) != JNI_OK ) {
        __android_log_print( ANDROID_LOG_INFO, "NTC", "GetEnv failed.\n" ) ;
        goto error ;
    }

    cls = env->FindClass(  "com/example/nativethreadcallback/MainActivity" ) ;
    if ( cls == NULL ) {
        __android_log_print( ANDROID_LOG_INFO, "NTC", "Native registration unable to find class(AVMJni)" ) ;
        goto error ;
    }

    if ( env->RegisterNatives(  cls, methods, sizeof( methods ) /
                                              sizeof ( methods[0] ) ) < 0 ) {
        __android_log_print(ANDROID_LOG_INFO, "NTC", "Registernatives failed !!!\n" ) ;
        goto error ;
    }
    result = JNI_VERSION_1_4 ;
    glpVM = vm ;
    error:
    return result ;
}
