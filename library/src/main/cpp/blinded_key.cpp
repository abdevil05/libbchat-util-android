#include <jni.h>
#include <bchat/blinding.hpp>
#include <bchat/util.hpp>

#include "util.h"
#include "jni_utils.h"

//
// Created by Thomas Ruffie on 29/7/2024.
//


extern "C"
JNIEXPORT jobject JNICALL
Java_org_bchatfoundation_libbchat_1util_util_BlindKeyAPI_blindVersionKeyPair(JNIEnv *env,
                                                                                  jobject thiz,
                                                                                  jbyteArray ed25519_secret_key) {
    return jni_utils::run_catching_cxx_exception_or_throws<jobject>(env, [=] {
        const auto [pk, sk] = bchat::blind_version_key_pair(util::vector_from_bytes(env, ed25519_secret_key));
        return jni_utils::new_key_pair(env, util::bytes_from_span(env, pk).get(), util::bytes_from_span(env, sk).get());
    });
}
extern "C"
JNIEXPORT jbyteArray JNICALL
Java_org_bchatfoundation_libbchat_1util_util_BlindKeyAPI_blindVersionSign(JNIEnv *env,
                                                                               jobject thiz,
                                                                               jbyteArray ed25519_secret_key,
                                                                               jlong timestamp) {
    return jni_utils::run_catching_cxx_exception_or_throws<jbyteArray>(env, [=] {
        auto bytes = bchat::blind_version_sign(
                jni_utils::JavaByteArrayRef(env, ed25519_secret_key).get(),
                bchat::Platform::android,
                timestamp
        );
        return util::bytes_from_vector(env, bytes).release();
    });
}

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_org_bchatfoundation_libbchat_1util_util_BlindKeyAPI_blindVersionSignRequest(JNIEnv *env,
                                                                                      jobject thiz,
                                                                                      jbyteArray ed25519_secret_key,
                                                                                      jlong timestamp,
                                                                                      jstring method,
                                                                                      jstring path,
                                                                                      jbyteArray body) {
    return jni_utils::run_catching_cxx_exception_or_throws<jbyteArray>(env, [=] {
        auto bytes = bchat::blind_version_sign_request(
                jni_utils::JavaByteArrayRef(env, ed25519_secret_key).get(),
                timestamp,
                jni_utils::JavaStringRef(env, method).view(),
                jni_utils::JavaStringRef(env, path).view(),
                body ? std::make_optional(jni_utils::JavaByteArrayRef(env, body).get()) : std::nullopt
        );
        return util::bytes_from_vector(env, bytes).release();
    });
}

extern "C"
JNIEXPORT jobject JNICALL
Java_org_bchatfoundation_libbchat_1util_util_BlindKeyAPI_blind15KeyPair(JNIEnv *env,
                                                                             jobject thiz,
                                                                             jbyteArray ed25519_secret_key,
                                                                             jbyteArray server_pub_key) {
    return jni_utils::run_catching_cxx_exception_or_throws<jobject>(env, [=] {
        auto [pk, sk] = bchat::blind15_key_pair(
                jni_utils::JavaByteArrayRef(env, ed25519_secret_key).get(),
                jni_utils::JavaByteArrayRef(env, server_pub_key).get()
                );
        return jni_utils::new_key_pair(env, util::bytes_from_span(env, pk).get(), util::bytes_from_span(env, sk).get());
    });
}

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_org_bchatfoundation_libbchat_1util_util_BlindKeyAPI_blind15Sign(JNIEnv *env, jobject thiz,
                                                                          jbyteArray ed25519_secret_key,
                                                                          jstring server_pub_key,
                                                                          jbyteArray message) {
    return jni_utils::run_catching_cxx_exception_or_throws<jbyteArray>(env, [=] {
        auto data = bchat::blind15_sign(
                jni_utils::JavaByteArrayRef(env, ed25519_secret_key).get(),
                jni_utils::JavaStringRef(env, server_pub_key).view(),
                jni_utils::JavaByteArrayRef(env, message).get()
                );
        return util::bytes_from_vector(env, data).release();
    });
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_org_bchatfoundation_libbchat_1util_util_BlindKeyAPI_bchatIdMatchesBlindedId(JNIEnv *env,
                                                                                        jobject thiz,
                                                                                        jstring bchat_id,
                                                                                        jstring blinded_id,
                                                                                        jstring server_pub_key) {
    return jni_utils::run_catching_cxx_exception_or<jboolean>([=]() -> jboolean {
        return bchat::bchat_id_matches_blinded_id(
                jni_utils::JavaStringRef(env, bchat_id).view(),
                jni_utils::JavaStringRef(env, blinded_id).view(),
                jni_utils::JavaStringRef(env, server_pub_key).view()
        );
    }, [](const char *) -> jboolean {
        return false;
    });
}

extern "C"
JNIEXPORT jobject JNICALL
Java_org_bchatfoundation_libbchat_1util_util_BlindKeyAPI_blind15Ids(JNIEnv *env, jobject thiz,
                                                                         jstring bchat_id,
                                                                         jstring server_pub_key) {
    return jni_utils::run_catching_cxx_exception_or_throws<jobject>(env, [=]() -> jobject {
        return jni_utils::jstring_list_from_collection(
                env,
                bchat::blind15_id(
                        jni_utils::JavaStringRef(env, bchat_id).view(),
                        jni_utils::JavaStringRef(env, server_pub_key).view()
                )
        );
    });
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_bchatfoundation_libbchat_1util_util_BlindKeyAPI_blind25Id(JNIEnv *env, jobject thiz,
                                                                        jstring bchat_id,
                                                                        jstring server_pub_key) {
    return jni_utils::run_catching_cxx_exception_or_throws<jstring>(env, [=]() -> jstring {
        return jni_utils::jstring_from_optional(env, bchat::blind25_id(
                jni_utils::JavaStringRef(env, bchat_id).view(),
                jni_utils::JavaStringRef(env, server_pub_key).view()
        )).release();
    });
}