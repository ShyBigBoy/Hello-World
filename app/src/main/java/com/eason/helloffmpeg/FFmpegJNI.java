package com.eason.helloffmpeg;

/**
 * Created by Sunny on 5/26/17.
 */

public class FFmpegJNI {
    static {
        try {
            System.loadLibrary("avcodec-57");
            System.loadLibrary("avfilter-6");
            System.loadLibrary("avformat-57");
            System.loadLibrary("avutil-55");
            System.loadLibrary("swresample-2");
            System.loadLibrary("swscale-4");
            System.loadLibrary("ffmpeghelloworld");
        } catch (UnsatisfiedLinkError ule) {}
    }

    public native static String native_avcodecInfo();
    public native static String native_stringFromJNI();
    public native static String native_avcodecConfiguration();
}
