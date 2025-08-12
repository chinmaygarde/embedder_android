// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_IMPELLER_TOOLKIT_EGL_CONTEXT_H_
#define FLUTTER_IMPELLER_TOOLKIT_EGL_CONTEXT_H_

#include <functional>

#include "egl_portable.h"

namespace embedder {

class Surface;
class Display;

//------------------------------------------------------------------------------
/// @brief      An instance of an EGL context.
///
///             An EGL context can only be used on a single thread at a given
///             time. A thread can only have a single context current at any
///             given time.
///
///             Context cannot be created directly. Only a valid instance of an
///             egl::Display can create a context.
///
class Context {
 public:
  ~Context();

  //----------------------------------------------------------------------------
  /// @brief      Determines if a valid context could be created. The context
  ///             still needs to be made current on the thread for it to be
  ///             useful.
  ///
  /// @return     True if valid, False otherwise.
  ///
  bool IsValid() const;

  //----------------------------------------------------------------------------
  /// @brief      Get the underlying handle to the EGL context.
  ///
  /// @return     The handle.
  ///
  const EGLContext& GetHandle() const;

  //----------------------------------------------------------------------------
  /// @brief      Make the context current on the calling thread. It is the
  ///             caller responsibility to ensure that any context previously
  ///             current on the thread must be cleared via `ClearCurrent`.
  ///
  /// @important  The config used to create the surface must match the config
  ///             used to create this context instance.
  ///
  /// @param[in]  surface  The surface to use to make the context current.
  ///
  /// @return     If the context could be made current on the callers thread.
  ///
  bool MakeCurrent(const Surface& surface) const;

  //----------------------------------------------------------------------------
  /// @brief      Clear the thread association of this context.
  ///
  /// @return     If the thread association could be cleared.
  ///
  bool ClearCurrent() const;

  //----------------------------------------------------------------------------
  /// @return     True if the context is current and attached to any surface,
  ///             False otherwise.
  ///
  bool IsCurrent() const;

 private:
  friend class Display;

  EGLDisplay display_ = EGL_NO_DISPLAY;
  EGLContext context_ = EGL_NO_CONTEXT;

  Context(EGLDisplay display, EGLContext context);

  Context(const Context&) = delete;

  Context& operator=(const Context&) = delete;
};

}  // namespace embedder

#endif  // FLUTTER_IMPELLER_TOOLKIT_EGL_CONTEXT_H_
