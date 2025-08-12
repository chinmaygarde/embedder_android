#pragma once

#include "flutter_embedder.h"
#include "macros.h"

namespace embedder {

class Engine {
 public:
  Engine();

  ~Engine();

  bool Launch();

  bool IsRunning() const;

 private:
  FlutterEngine engine_ = {};

  bool MakeCurrent();

  bool ClearCurrent();

  bool Present();

  uint32_t GetOnscreenFBO();

  void* GetProcAddress(const char* proc_name);

  EM_DISALLOW_COPY_ASSIGN_AND_MOVE(Engine);
};

}  // namespace embedder
