#include <android/log.h>
#include <android/native_activity.h>
#include <jni.h>
#include <stdlib.h>
#include "logging.h"

__attribute__((visibility("default"))) void ANativeActivity_onCreate(
    ANativeActivity* activity,
    void* savedState,
    size_t savedStateSize) {
  LOG_FATAL << "Hello";
}
