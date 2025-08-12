#include "engine.h"
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include "logging.h"

namespace embedder {

Engine::Engine() {}

Engine::~Engine() {
  if (engine_) {
    FlutterEngineShutdown(engine_);
  }
}

static const char* FlutterEngineResultToString(FlutterEngineResult result) {
  switch (result) {
    case kSuccess:
      return "Success";
    case kInvalidLibraryVersion:
      return "Invalid Library Version";
    case kInvalidArguments:
      return "Invalid Arguments";
    case kInternalInconsistency:
      return "Internal Inconsistency";
  }
  return "Unknown";
}

bool Engine::Launch() {
  if (IsRunning()) {
    LOG_ERROR << "Engine is already running.";
    return false;
  }
  FlutterRendererConfig renderer = {};
  renderer.type = kOpenGL;
  renderer.open_gl.struct_size = sizeof(renderer.open_gl);
  renderer.open_gl.make_current = [](void* userdata) -> bool {
    return reinterpret_cast<Engine*>(userdata)->MakeCurrent();
  };
  renderer.open_gl.clear_current = [](void* userdata) -> bool {
    return reinterpret_cast<Engine*>(userdata)->ClearCurrent();
  };
  renderer.open_gl.present = [](void* userdata) -> bool {
    return reinterpret_cast<Engine*>(userdata)->Present();
  };
  renderer.open_gl.fbo_callback = [](void* userdata) -> uint32_t {
    return reinterpret_cast<Engine*>(userdata)->GetOnscreenFBO();
  };
  renderer.open_gl.gl_proc_resolver = [](void* userdata,
                                         const char* proc_name) -> void* {
    return reinterpret_cast<Engine*>(userdata)->GetProcAddress(proc_name);
  };

  FlutterProjectArgs args = {};
  args.struct_size = sizeof(args);

  FlutterEngine engine = nullptr;
  FlutterEngineResult result = FlutterEngineRun(FLUTTER_ENGINE_VERSION,  //
                                                &renderer,               //
                                                &args,                   //
                                                this,                    //
                                                &engine                  //
  );
  if (result != kSuccess || engine == nullptr) {
    LOG_ERROR << "Could not launch the Flutter engine: "
              << FlutterEngineResultToString(result);
    return false;
  }
  engine_ = engine;
  SetSurfaceSize();
  return true;
}

bool Engine::MakeCurrent() {
  return delegate_ ? delegate_->GLRenderContextMakeCurrent() : false;
}

bool Engine::ClearCurrent() {
  return delegate_ ? delegate_->GLContextClearCurrent() : false;
}

bool Engine::Present() {
  return delegate_ ? delegate_->GLContextPresent() : false;
}

uint32_t Engine::GetOnscreenFBO() {
  return delegate_ ? delegate_->GLRenderContextFBO() : 0u;
}

void* Engine::GetProcAddress(const char* proc_name) {
  return delegate_ ? delegate_->GLGetProcAddress(proc_name) : nullptr;
}

bool Engine::IsRunning() const {
  return engine_ != nullptr;
}

void Engine::SetDelegate(EngineDelegate* delegate) {
  delegate_ = delegate;
}

void Engine::SetSurfaceSize(FlutterUIntSize size) {
  window_size_ = size;
  if (!engine_) {
    return;
  }
  FlutterWindowMetricsEvent metrics = {};
  metrics.struct_size = sizeof(metrics);
  metrics.width = window_size_.width;
  metrics.height = window_size_.height;
  metrics.pixel_ratio = 1.0;
  auto result = FlutterEngineSendWindowMetricsEvent(engine_, &metrics);
  if (result != kSuccess) {
    LOG_ERROR << "Could not dispatch window metrics: "
              << FlutterEngineResultToString(result);
  }
}

void Engine::SetSurfaceSize() {
  SetSurfaceSize(window_size_);
}

}  // namespace embedder
