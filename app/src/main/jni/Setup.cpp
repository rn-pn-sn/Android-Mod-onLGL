#include <jni.h>
#include "Includes/obfuscate.h"

namespace ToastLength {
    inline const int LENGTH_LONG = 1;
    inline const int LENGTH_SHORT = 0;
}

void Toast(JNIEnv *env, jobject thiz, const char *text, int length) {
    jstring jstr = env->NewStringUTF(text);
    jclass toast = env->FindClass(OBFUSCATE("android/widget/Toast"));
    jmethodID methodMakeText =env->GetStaticMethodID(toast,OBFUSCATE("makeText"),OBFUSCATE("(Landroid/content/Context;Ljava/lang/CharSequence;I)Landroid/widget/Toast;"));
    jobject toastobj = env->CallStaticObjectMethod(toast, methodMakeText,thiz, jstr, length);
    jmethodID methodShow = env->GetMethodID(toast, OBFUSCATE("show"), OBFUSCATE("()V"));
    env->CallVoidMethod(toastobj, methodShow);
}

void Dialog(JNIEnv *env, jobject context, const char *title, const char *message, const char *openBtn, const char *closeBtn, int sec, const char *url) {
    jclass dialogHelperClass = env->FindClass(OBFUSCATE("com/android/support/DialogHelper"));
    jmethodID showMethod = env->GetStaticMethodID(dialogHelperClass, OBFUSCATE("showDialogWithLink"),
                                                  OBFUSCATE("(Landroid/content/Context;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;)V"));

    jstring jTitle = env->NewStringUTF(title);
    jstring jMessage = env->NewStringUTF(message);
    jstring jOpen = env->NewStringUTF(openBtn);
    jstring jClose = env->NewStringUTF(closeBtn);
    jint jSec = sec;
    jstring jUrl = env->NewStringUTF(url);

    env->CallStaticVoidMethod(dialogHelperClass, showMethod, context, jTitle, jMessage, jOpen, jClose, jSec, jUrl);

    env->DeleteLocalRef(jTitle);
    env->DeleteLocalRef(jMessage);
    env->DeleteLocalRef(jOpen);
    env->DeleteLocalRef(jClose);
    env->DeleteLocalRef(jUrl);
}

void Init(JNIEnv *env, jobject thiz, jobject ctx) {
    //Dialog Example
    Dialog(env,
           ctx,
           OBFUSCATE("Welcome to your mod menu"),
           OBFUSCATE("Thanks for the installation and call to action"),
           OBFUSCATE("Visit"), // set nullptr if btn-link is not needed
           OBFUSCATE("Close"),
           3, // set 0 if auto-close is not needed
           OBFUSCATE("https://github.com/LGLTeam")); // set nullptr if btn-link is not needed

    // Toast Example
    Toast(env, ctx, OBFUSCATE("Modded by YOU"), ToastLength::LENGTH_LONG);
}

int RegisterMain(JNIEnv *env) {
    JNINativeMethod methods[] = {
            {OBFUSCATE("Init"),            OBFUSCATE(
                    "(Landroid/content/Context;)V"), reinterpret_cast<void *>(Init)},
    };
    jclass clazz = env->FindClass(OBFUSCATE("com/android/support/MainActivity"));
    if (!clazz)
        return JNI_ERR;
    if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) != 0)
        return JNI_ERR;

    return JNI_OK;
}

extern "C"
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    vm->GetEnv((void **) &env, JNI_VERSION_1_6);
    if (RegisterMain(env) != 0)
        return JNI_ERR;
    return JNI_VERSION_1_6;
}