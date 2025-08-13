#pragma once

#include <android/native_activity.h>
#include <map>
#include "egl/context.h"
#include "egl/display.h"
#include "egl/surface.h"
#include "engine.h"
#include "macros.h"

namespace embedder {

class Activity final : public EngineDelegate {
 public:
  Activity(ANativeActivity* activity);

  ~Activity();

 private:
  Display display_;
  std::map<ANativeWindow*, std::shared_ptr<Surface>> surfaces_;
  std::unique_ptr<Context> context_;
  std::unique_ptr<Engine> engine_;
  bool is_valid_ = false;

  bool InstallCallbacks(ANativeActivity* activity);

  void OnStart();

  void OnResume();

  void OnPause();

  void OnStop();

  void OnNativeWindowCreated(ANativeWindow* window);

  void OnNativeWindowResize(ANativeWindow* window);

  void OnNativeWindowDestroyed(ANativeWindow* window);

  void OnLowMemory();

  bool StartEngine();

  bool StopEngine();

  void UpdateEngineSurfaceSize();

  // |EngineDelegate|
  bool GLRenderContextMakeCurrent();

  // |EngineDelegate|
  bool GLContextClearCurrent();

  // |EngineDelegate|
  bool GLContextPresent();

  // |EngineDelegate|
  uint32_t GLRenderContextFBO();

  // |EngineDelegate|
  void* GLGetProcAddress(const char* proc_name);

  EM_DISALLOW_COPY_ASSIGN_AND_MOVE(Activity);
};

}  // namespace embedder
