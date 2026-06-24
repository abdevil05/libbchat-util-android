#ifndef BCHAT_ANDROID_CONFIG_BASE_H
#define BCHAT_ANDROID_CONFIG_BASE_H

#include <bchat/config/base.hpp>

#include <jni.h>

bchat::config::ConfigBase* ptrToConfigBase(JNIEnv *env, jobject obj);
bchat::config::ConfigSig* ptrToConfigSig(JNIEnv* env, jobject obj);

#endif
