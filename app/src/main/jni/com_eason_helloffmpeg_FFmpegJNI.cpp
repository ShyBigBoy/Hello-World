//
// Created by Sunny on 5/26/17.
//

#define LOG_TAG "FFmpegJNI"

#include <stdio.h>
#include <assert.h>
#include <jni.h>
#include <errno.h>
#include <string.h>

//pirnt log
#ifdef ANDROID
#include <android/log.h>
#define LOGI(msg...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, msg)
#define LOGE(msg...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, msg)
#else
#define LOGI(format, ...) printf("(>_<) " format "\n", __VA_ARGS__)
#define LOGE(format, ...) printf("(>_<) " format "\n", __VA_ARGS__)
#define LOGE(...) printf("(>_<)\n", __VA_ARGS__)
#endif

#ifndef NELEM
# define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavfilter/avfilter.h"

static jstring avcodecInfo(JNIEnv *env, jobject obj) {
    char info[40000] = {0};
    av_register_all();
    AVCodec *c_temp = av_codec_next(NULL);
    while (c_temp != NULL) {
        if (c_temp->decode != NULL) { sprintf(info, "%s[Dec]", info); }
        else { sprintf(info, "%s[Enc]", info); }
        switch (c_temp->type) {
            case AVMEDIA_TYPE_VIDEO:
                sprintf(info, "%s[Video]", info);
                break;
            case AVMEDIA_TYPE_AUDIO:
                sprintf(info, "%s[Audio]", info);
                break;
            default:
                sprintf(info, "%s[Other]", info);
                break;
        }
        sprintf(info, "%s[%10s]\n", info, c_temp->name);
        c_temp = c_temp->next;
    }
    LOGI("%s", info);
    return env->NewStringUTF(info);
}

static jstring stringFromJNI(JNIEnv *env, jobject thiz) {
#if defined(__arm__)
    #if defined(__ARM_ARCH_7A__)
        #if defined(__ARM_NEON__)
            #define ABI "armeabi-v7a/NEON"
        #else
            #define ABI "armeabi-v7a"
        #endif
    #else
        #define ABI "armeabi"
    #endif
#elif defined(__i386__)
   #define ABI "x86"
#elif defined(__mips__)
   #define ABI "mips"
#else
   #define ABI "unknown"
#endif

    return env->NewStringUTF("Hello from JNI !  Compiled with ABI=" ABI ".");
}

static jstring avcodecConfiguration(JNIEnv *env, jobject thiz) {
    char info[10000] = { 0 };
    sprintf(info, "%s\n", avcodec_configuration());
    return env->NewStringUTF(info);
}

static const char *classPathName = "com/eason/helloffmpeg/FFmpegJNI";
static JNINativeMethod g_methods[] = {
    { "native_avcodecInfo", "()Ljava/lang/String;", (void*)avcodecInfo },
    { "native_stringFromJNI", "()Ljava/lang/String;", (void*)stringFromJNI },
    { "native_avcodecConfiguration", "()Ljava/lang/String;", (void*)avcodecConfiguration }
};

/*int register_com_eason_helloffmpeg(JNIEnv *env) {
    return registerNativeMethods(env, classPathName, g_methods, NELEM(g_methods));
}*/

static int registerNativeMethods(JNIEnv* env, const char* className, JNINativeMethod* gMethods, int numMethods) {
    jclass clazz;
    clazz = env->FindClass(className);
    if (clazz == NULL) {
        LOGE("Native registration unable to find class '%s'", className);
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        LOGE("RegisterNatives failed for '%s'", className);
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

jint JNI_OnLoad(JavaVM *vm, void* reserved) {
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        LOGE("Error: GetEnv failed in JNI_OnLoad");
        return -1;
    }
    assert(env != NULL);
    if (!registerNativeMethods(env, classPathName, g_methods, NELEM(g_methods))) {
        LOGE("Error: could not register native methods for FFmpegJNI");
        return -1;
    }
    return JNI_VERSION_1_6;
}

#ifdef __cplusplus
}
#endif
