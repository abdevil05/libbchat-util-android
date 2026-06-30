#include <jni.h>
#include <android/log.h>

// This file documents and guards an important lifecycle assumption made
// throughout this codebase: every JNI bridge function in this library uses
// function-local `static BasicJavaClassInfo` / `static JavaClassInfo`
// instances to cache jclass global references and jmethodID/jfieldID
// lookups on first use (see jni_utils.h).
//
// This caching pattern is safe and efficient *as long as this native
// library is loaded exactly once per process and never unloaded*. This is
// true for the overwhelming majority of real Android app lifecycles, since
// System.loadLibrary("bchat_util") is typically called once from a
// singleton object's init block and the .so is not explicitly unloaded.
//
// If this library were ever unloaded and reloaded within the same process
// (a rare scenario, generally only reachable through unusual multi-
// classloader setups, plugin frameworks, or test harnesses that load/unload
// native libraries repeatedly), the function-local statics would NOT be
// re-initialized, since C++ guarantees one-time initialization for static
// locals. They would continue to hold global references tied to the
// *previous* classloader, which is undefined behavior and can crash the
// JVM.
//
// JNI_OnLoad/JNI_OnUnload are provided here primarily so that:
//   1. This assumption is documented at a well-known, discoverable location.
//   2. JNI_OnUnload logs a clear diagnostic warning if unload is ever
//      observed, rather than failing silently or crashing without context.
//   3. Future maintainers have an obvious place to add eager
//      class-info initialization if this assumption ever needs to change.

#define LOG_TAG "bchat_util_jni_init"

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG,
        "libbchat_util loaded. Note: JNI class-info caches are lazily "
        "initialized per-function and assume this library is never "
        "unloaded/reloaded within this process.");
    return JNI_VERSION_1_6;
}

extern "C" JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved) {
    __android_log_print(ANDROID_LOG_WARN, LOG_TAG,
        "libbchat_util is being unloaded. Cached static JNI class-info "
        "instances throughout this codebase hold global references tied "
        "to the classloader that is now being torn down. If this library "
        "is reloaded in this process, those cached references will be "
        "stale and can cause crashes. This scenario was not expected "
        "when this library was designed - investigate why unload "
        "occurred.");
}
