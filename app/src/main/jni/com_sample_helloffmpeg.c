//
// Created by Sunny on 5/24/17.
//
#include <stdio.h>
#include "com_eason_helloffmpeg_MainActivity.h"

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavfilter/avfilter.h"

//pirnt log
#ifdef ANDROID
#include <jni.h>
#include <android/log.h>
#define ALOGE(format, ...) __android_log_print(ANDROID_LOG_ERROR, "simplest_ffmpeg", format, __VA_ARGS__)
#else
#define ALOGE(format, ...) printf("(>_<) " format "\n", __VA_ARGS__)
#endif

JNIEXPORT jstring JNICALL Java_com_eason_helloffmpeg_MainActivity_avcodecInfo(JNIEnv *env, jobject obj) {
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
    ALOGE("%s", info);
    return (*env)->NewStringUTF(env, info);
}

JNIEXPORT jstring JNICALL Java_com_eason_helloffmpeg_MainActivity_stringFromJNI(JNIEnv *env, jobject thiz) {
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

    return (*env)->NewStringUTF(env, "Hello from JNI !  Compiled with ABI=" ABI ".");
}

JNIEXPORT jstring JNICALL Java_com_eason_helloffmpeg_MainActivity_avcodecConfiguration(JNIEnv *env, jobject thiz) {
    char info[10000] = { 0 };
    sprintf(info, "%s\n", avcodec_configuration());
    return (*env)->NewStringUTF(env, info);
}