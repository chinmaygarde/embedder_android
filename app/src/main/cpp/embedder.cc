#include <jni.h>
#include <string>
#include <android/native_activity.h>
#include <android/log.h>

__attribute__((visibility("default"))) void ANativeActivity_onCreate(ANativeActivity* activity,
        void* savedState, size_t savedStateSize) {
    __android_log_print(ANDROID_LOG_FATAL, "embedder", "Hello2");
    abort();
}
