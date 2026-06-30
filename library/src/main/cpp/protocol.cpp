#include <jni.h>
#include <bchat/bchat_protocol.hpp>
#include <bchat/sodium_array.hpp>

#include "jni_utils.h"
#include "pro_proof_util.h"
#include "util.h"

using namespace jni_utils;

static JavaLocalRef<jobject> serializeDecodedPro(JNIEnv *env, const bchat::DecodedPro &pro) {
    static BasicJavaClassInfo class_info(
            env,
            "org/bchatfoundation/libbchat_util/protocol/DecodedPro",
            "(ILorg/bchatfoundation/libbchat_util/pro/ProProof;JJ)V"
    );

    return {env, env->NewObject(class_info.java_class, class_info.constructor,
                          static_cast<jint>(pro.status),
                          cpp_to_java_proof(env, pro.proof).get(),
                          static_cast<jlong>(pro.msg_bitset.data),
                          static_cast<jlong>(pro.profile_bitset.data))
    };
}

static JavaLocalRef<jobject> serializeEnvelop(JNIEnv *env, const bchat::Envelope &envelope) {
    static BasicJavaClassInfo class_info(
            env,
            "org/bchatfoundation/libbchat_util/protocol/Envelope",
            "(J[BJ[B)V");

    return {env, env->NewObject(class_info.java_class,
                                class_info.constructor,
                                static_cast<jlong>(envelope.timestamp.count()),
                                (envelope.flags & BCHAT_PROTOCOL_ENVELOPE_FLAGS_SOURCE)
                                ? util::bytes_from_span(env, envelope.source).get()
                                : nullptr,
                                (envelope.flags & BCHAT_PROTOCOL_ENVELOPE_FLAGS_SERVER_TIMESTAMP)
                                ? static_cast<jlong>(envelope.server_timestamp)
                                : 0,
                                util::bytes_from_span(env, envelope.pro_sig).get())};
}

static JavaLocalRef<jobject> serializeDecodedEnvelope(JNIEnv *env, const bchat::DecodedEnvelope &envelop) {
    static BasicJavaClassInfo class_info(
            env,
            "org/bchatfoundation/libbchat_util/protocol/DecodedEnvelope",
            "(Lorg/bchatfoundation/libbchat_util/protocol/Envelope;Lorg/bchatfoundation/libbchat_util/protocol/DecodedPro;[B[B[BJ)V"
    );

    JavaLocalRef sender_ed25519 = util::bytes_from_span(env, envelop.sender_ed25519_pubkey);
    JavaLocalRef sender_x25519 = util::bytes_from_span(env, envelop.sender_x25519_pubkey);
    JavaLocalRef content = util::bytes_from_vector(env, envelop.content_plaintext);

    return {env, env->NewObject(class_info.java_class, class_info.constructor,
                          serializeEnvelop(env, envelop.envelope).get(),
                          envelop.pro ? serializeDecodedPro(env, *envelop.pro).get() : nullptr,
                          content.get(),
                          sender_ed25519.get(),
                          sender_x25519.get(),
                          static_cast<jlong>(envelop.envelope.timestamp.count()))};

}

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_org_bchatfoundation_libbchat_1util_protocol_BChatProtocol_encodeFor1o1(JNIEnv *env,
                                                                                   jobject thiz,
                                                                                   jbyteArray plaintext,
                                                                                   jbyteArray my_ed25519_priv_key,
                                                                                   jlong timestamp_ms,
                                                                                   jbyteArray recipient_pub_key,
                                                                                   jbyteArray rotating_key) {
    return run_catching_cxx_exception_or_throws<jbyteArray>(env, [=] {
        return util::bytes_from_vector(
                env,
                bchat::encode_for_1o1(
                        JavaByteArrayRef(env, plaintext).get(),
                        JavaByteArrayRef(env, my_ed25519_priv_key).get(),
                        std::chrono::milliseconds{timestamp_ms},
                        *java_to_cpp_array<33>(env, recipient_pub_key),
                        rotating_key ? std::optional(JavaByteArrayRef(env, rotating_key).get())
                                     : std::nullopt
                )
        ).release();
    });
}

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_org_bchatfoundation_libbchat_1util_protocol_BChatProtocol_encodeForCommunityInbox(
        JNIEnv *env, jobject thiz, jbyteArray plaintext, jbyteArray my_ed25519_priv_key,
        jlong timestamp_ms, jbyteArray recipient_pub_key, jbyteArray community_server_pub_key,
        jbyteArray rotating_key) {
    return run_catching_cxx_exception_or_throws<jbyteArray>(env, [=] {
        return util::bytes_from_vector(
                env,
                bchat::encode_for_community_inbox(
                        JavaByteArrayRef(env, plaintext).get(),
                        JavaByteArrayRef(env, my_ed25519_priv_key).get(),
                        std::chrono::milliseconds{timestamp_ms},
                        *java_to_cpp_array<33>(env, recipient_pub_key),
                        *java_to_cpp_array<32>(env, community_server_pub_key),
                        rotating_key ? std::optional(JavaByteArrayRef(env, rotating_key).get())
                                     : std::nullopt
                )
        ).release();
    });
}

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_org_bchatfoundation_libbchat_1util_protocol_BChatProtocol_encodeForGroup(JNIEnv *env,
                                                                                     jobject thiz,
                                                                                     jbyteArray plaintext,
                                                                                     jbyteArray my_ed25519_priv_key,
                                                                                     jlong timestamp_ms,
                                                                                     jbyteArray group_ed25519_public_key,
                                                                                     jbyteArray group_ed25519_private_key,
                                                                                     jbyteArray rotating_key) {
    return run_catching_cxx_exception_or_throws<jbyteArray>(env, [=] {
        bchat::cleared_uc32 group_private_key;

        auto array = *java_to_cpp_array<32>(env, group_ed25519_private_key);
        std::copy(array.begin(), array.end(), group_private_key.begin());

        return util::bytes_from_vector(
                env,
                bchat::encode_for_group(
                        JavaByteArrayRef(env, plaintext).get(),
                        JavaByteArrayRef(env, my_ed25519_priv_key).get(),
                        std::chrono::milliseconds{timestamp_ms},
                        *java_to_cpp_array<33>(env, group_ed25519_public_key),
                        group_private_key,
                        rotating_key ? std::optional(JavaByteArrayRef(env, rotating_key).get())
                                     : std::nullopt
                )
        ).release();
    });
}

extern "C"
JNIEXPORT jobject JNICALL
Java_org_bchatfoundation_libbchat_1util_protocol_BChatProtocol_decodeForCommunity(
        JNIEnv *env, jobject thiz, jbyteArray payload, jlong timestamp_ms,
        jbyteArray pro_backend_pub_key) {
    return run_catching_cxx_exception_or_throws<jobject>(env, [=] {
        jni_utils::JavaByteArrayRef payload_ref(env, payload);

        auto decoded = bchat::decode_for_community(
                payload_ref.get(),
                std::chrono::sys_time<std::chrono::milliseconds>{
                        std::chrono::milliseconds{timestamp_ms}},
                *java_to_cpp_array<32>(env, pro_backend_pub_key)
        );

        static BasicJavaClassInfo class_info(
                env,
                "org/bchatfoundation/libbchat_util/protocol/DecodedCommunityMessage",
                "(Lorg/bchatfoundation/libbchat_util/protocol/DecodedPro;[B)V"
        );

        return env->NewObject(
                class_info.java_class,
                class_info.constructor,
                decoded.pro ? serializeDecodedPro(env, *decoded.pro).get() : nullptr,
                util::bytes_from_vector(env, decoded.content_plaintext).get()
        );
    });
}

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_org_bchatfoundation_libbchat_1util_protocol_BChatProtocol_encodeForCommunity(
        JNIEnv *env,
        jobject thiz,
        jbyteArray plaintext,
        jbyteArray rotating_key) {
    return run_catching_cxx_exception_or_throws<jbyteArray>(env, [=] {
        return util::bytes_from_vector(
                env,
                bchat::encode_for_community(
                        JavaByteArrayRef(env, plaintext).get(),
                        rotating_key ? std::optional(JavaByteArrayRef(env, rotating_key).get())
                                     : std::nullopt
                )
        ).release();
    });
}

extern "C"
JNIEXPORT jobject JNICALL
Java_org_bchatfoundation_libbchat_1util_protocol_BChatProtocol_decodeFor1o1(JNIEnv *env,
                                                                                   jobject thiz,
                                                                                   jbyteArray key,
                                                                                   jbyteArray payload,
                                                                                   jbyteArray pro_backend_pub_key) {
    return run_catching_cxx_exception_or_throws<jobject>(env, [=] {
        JavaByteArrayRef key_ref(env, key);

        std::array<std::span<const uint8_t>, 1> keys = {key_ref.get()};

        bchat::DecodeEnvelopeKey decode_key{
                .decrypt_keys = std::span(keys.data(), keys.size()),
        };

        return serializeDecodedEnvelope(env, bchat::decode_envelope(
                decode_key,
                JavaByteArrayRef(env, payload).get(),
                *java_to_cpp_array<32>(env, pro_backend_pub_key)
        )).release();
    });
}


extern "C"
JNIEXPORT jobject JNICALL
Java_org_bchatfoundation_libbchat_1util_protocol_BChatProtocol_decodeForGroup(JNIEnv *env,
                                                                                     jobject thiz,
                                                                                     jbyteArray payload,
                                                                                     jbyteArray my_ed25519_priv_key,
                                                                                     jbyteArray group_ed25519_public_key,
                                                                                     jobjectArray group_ed25519_private_keys,
                                                                                     jbyteArray pro_backend_pub_key) {
    return run_catching_cxx_exception_or_throws<jobject>(env, [=] {
        std::vector<JavaByteArrayRef> private_keys_refs;
        std::vector<std::span<const uint8_t>> private_keys_spans;
        for (int i = 0, size = env->GetArrayLength(group_ed25519_private_keys); i < size; i++) {
            auto bytes = reinterpret_cast<jbyteArray>(env->GetObjectArrayElement(
                    group_ed25519_private_keys, i));
            private_keys_spans.emplace_back(private_keys_refs.emplace_back(env, bytes).get());
        }

        JavaByteArrayRef group_pub_key_ref(env, group_ed25519_public_key);

        bchat::DecodeEnvelopeKey decode_key{
                .group_ed25519_pubkey = std::make_optional(group_pub_key_ref.get()),
                .decrypt_keys = std::span(private_keys_spans.data(), private_keys_spans.size()),
        };

        return serializeDecodedEnvelope(env, bchat::decode_envelope(
                decode_key,
                JavaByteArrayRef(env, payload).get(),
                *java_to_cpp_array<32>(env, pro_backend_pub_key)
        )).release();
    });
}
