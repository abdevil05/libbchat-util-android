#include "jni_utils.h"

#include <bchat/hash.hpp>

extern "C"
JNIEXPORT void JNICALL
Java_network_loki_messenger_libbchat_1util_Hash_hash(JNIEnv *env, jobject thiz,
                                                       jbyteArray message,
                                                       jbyteArray hashOut,
                                                       jbyteArray key) {
    jni_utils::run_catching_cxx_exception_or_throws<void>(env, [=] {
        bchat::hash::hash(
                jni_utils::JavaByteArrayRef(env, hashOut).get(),
                jni_utils::JavaByteArrayRef(env, message).get(),
                key ? std::make_optional(jni_utils::JavaByteArrayRef(env, key).get()) : std::nullopt);
    });
}