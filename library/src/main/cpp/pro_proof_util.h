#pragma once

#include <bchat/bchat_protocol.hpp>
#include <jni.h>

#include "jni_utils.h"

bchat::ProProof java_to_cpp_proof(JNIEnv *, jobject proof);
jni_utils::JavaLocalRef<jobject> cpp_to_java_proof(JNIEnv *, const bchat::ProProof &proof);