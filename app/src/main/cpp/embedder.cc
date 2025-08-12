#include <android/log.h>
#include <android/native_activity.h>
#include <flutter_embedder.h>
#include <jni.h>
#include <stdlib.h>
#include "logging.h"

namespace embedder {

EM_EXTERN_C EM_VISIBILITY_DEFAULT void ANativeActivity_onCreate(
    ANativeActivity* activity,
    void* saved_state,
    size_t saved_state_size) {
  LOG_FATAL << FlutterEngineRunsAOTCompiledDartCode();
}

}  // namespace embedder
