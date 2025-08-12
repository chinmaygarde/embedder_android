#include "engine.h"
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include "logging.h"

namespace embedder {

Engine::Engine() {}

Engine::~Engine() {}

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
    LOG_ERROR << "Could not launch the Flutter engine.";
    return false;
  }
  engine_ = engine;
  return true;
}

bool Engine::MakeCurrent() {
  return false;
}

bool Engine::ClearCurrent() {
  return false;
}

bool Engine::Present() {
  return false;
}

uint32_t Engine::GetOnscreenFBO() {
  return 0;
}

void* Engine::GetProcAddress(const char* proc_name) {
  return nullptr;
}

bool Engine::IsRunning() const {
  return !!engine_;
}

}  // namespace embedder
