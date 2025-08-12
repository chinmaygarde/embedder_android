#pragma once

#include "flutter_embedder.h"
#include "macros.h"

namespace embedder {

class EngineDelegate {
 public:
  virtual bool GLRenderContextMakeCurrent() = 0;

  virtual bool GLContextClearCurrent() = 0;

  virtual bool GLContextPresent() = 0;

  virtual uint32_t GLRenderContextFBO() = 0;

  virtual void* GLGetProcAddress(const char* proc_name) = 0;
};

class Engine {
 public:
  Engine();

  ~Engine();

  bool Launch();

  bool IsRunning() const;

  void SetDelegate(EngineDelegate* delegate);

  void SetSurfaceSize(FlutterUIntSize size);

  void SetSurfaceSize();

 private:
  FlutterEngine engine_ = {};
  EngineDelegate* delegate_ = {};
  FlutterUIntSize window_size_ = {};

  bool MakeCurrent();

  bool ClearCurrent();

  bool Present();

  uint32_t GetOnscreenFBO();

  void* GetProcAddress(const char* proc_name);

  EM_DISALLOW_COPY_ASSIGN_AND_MOVE(Engine);
};

}  // namespace embedder
