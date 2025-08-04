#pragma once

#include "embedder.h"
#include "macros.h"

namespace embedder {

class Engine {
 public:
  Engine();

  ~Engine();

 private:
  FlutterEngineProcTable proc_table_ = {};
  bool is_valid_ = false;

  EM_DISALLOW_COPY_ASSIGN_AND_MOVE(Engine);
};

}  // namespace embedder
