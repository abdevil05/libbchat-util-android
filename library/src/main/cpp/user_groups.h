
#ifndef BCHAT_ANDROID_USER_GROUPS_H
#define BCHAT_ANDROID_USER_GROUPS_H

#include "jni_utils.h"
#include <bchat/config/user_groups.hpp>

jni_utils::JavaLocalRef<jobject> serialize_base_community(JNIEnv *env, const bchat::config::community& base_community);
bchat::config::community deserialize_base_community(JNIEnv *env, jobject base_community);

#endif //BCHAT_ANDROID_USER_GROUPS_H
