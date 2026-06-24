#ifndef BCHAT_ANDROID_UTIL_H
#define BCHAT_ANDROID_UTIL_H

#include <jni.h>
#include <array>
#include <optional>
#include <span>
#include <vector>

#include "bchat/types.hpp"
#include "bchat/config/groups/info.hpp"
#include "bchat/config/groups/keys.hpp"
#include "bchat/config/groups/members.hpp"
#include "bchat/config/profile_pic.hpp"
#include "bchat/config/user_groups.hpp"
#include "bchat/config/expiring.hpp"

#include "jni_utils.h"

namespace util {
    jni_utils::JavaLocalRef<jbyteArray> bytes_from_vector(JNIEnv* env, const std::vector<unsigned char> &from_str);
    std::vector<unsigned char> vector_from_bytes(JNIEnv* env, jbyteArray byteArray);
    jni_utils::JavaLocalRef<jbyteArray> bytes_from_span(JNIEnv* env, std::span<const unsigned char> from_str);
    jni_utils::JavaLocalRef<jobject> serialize_user_pic(JNIEnv *env, bchat::config::profile_pic pic);
    bchat::config::profile_pic deserialize_user_pic(JNIEnv *env, jobject user_pic);

    jni_utils::JavaLocalRef<jobject> serialize_expiry(JNIEnv *env, const bchat::config::expiration_mode& mode, const std::chrono::seconds& time_seconds);
    std::pair<bchat::config::expiration_mode, long> deserialize_expiry(JNIEnv *env, jobject expiry_mode);

    jni_utils::JavaLocalRef<jobject> jlongFromOptional(JNIEnv* env, std::optional<long long> optional);
}

#endif
