#pragma once

#define EM_DISALLOW_COPY_ASSIGN_AND_MOVE(TypeName) \
  TypeName(const TypeName&) = delete;              \
  TypeName(TypeName&&) = delete;                   \
  TypeName& operator=(const TypeName&) = delete;   \
  TypeName& operator=(TypeName&&) = delete

#define EM_EXTERN_C extern "C"
#define EM_VISIBILITY_DEFAULT __attribute__((visibility("default")))

namespace embedder {}
