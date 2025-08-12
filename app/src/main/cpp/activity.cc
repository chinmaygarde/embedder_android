#include "activity.h"

#include "logging.h"

namespace embedder {

Activity::Activity(ANativeActivity* activity) {
  if (!InstallCallbacks(activity)) {
    return;
  }
  is_valid_ = true;
}

Activity::~Activity() {}

void Activity::OnStart() {}

void Activity::OnResume() {}

void Activity::OnPause() {}

void Activity::OnStop() {}

void Activity::OnNativeWindowCreated(ANativeWindow* window) {
  if (surfaces_.find(window) != surfaces_.end()) {
    LOG_ERROR << "Surface for window already exists.";
    return;
  }

  if (surfaces_.size() == 1u) {
    LOG_ERROR << "Embedder only supports one window.";
    return;
  }

  if (!display_.IsValid()) {
    LOG_ERROR << "Invalid display.";
    return;
  }

  ConfigDescriptor desc;
  desc.api = API::kOpenGLES3;
  desc.samples = Samples::kOne;
  desc.color_format = ColorFormat::kRGBA8888;
  desc.depth_bits = DepthBits::kZero;
  desc.stencil_bits = StencilBits::kZero;
  desc.surface_type = SurfaceType::kWindow;

  auto config = display_.ChooseConfig(desc);
  if (!config) {
    LOG_ERROR << "Could not choose config.";
    return;
  }

  if (!context_) {
    context_ = display_.CreateContext(*config, nullptr);
  }

  if (!context_) {
    LOG_ERROR << "Could not create context.";
    return;
  }

  auto surface = display_.CreateWindowSurface(*config, window);
  if (!surface) {
    LOG_ERROR << "Could not create surface.";
    return;
  }

  surfaces_[window] = std::move(surface);
  StartEngine();
}

void Activity::OnNativeWindowResize(ANativeWindow* window) {
  if (surfaces_.find(window) == surfaces_.end()) {
    LOG_ERROR << "Embedder only supports one window.";
    return;
  }
  UpdateEngineSurfaceSize();
}

void Activity::OnNativeWindowDestroyed(ANativeWindow* window) {
  auto erased = surfaces_.erase(window) == 1u;
  if (!erased) {
    LOG_ERROR << "Embedder only supports rendering to one window.";
    return;
  }
  StopEngine();
}

void Activity::OnLowMemory() {}

bool Activity::InstallCallbacks(ANativeActivity* activity) {
#define ACT reinterpret_cast<Activity*>(activity->instance)
  activity->instance = this;
  activity->callbacks->onStart = [](ANativeActivity* activity) {
    return ACT->OnStart();
  };
  activity->callbacks->onResume = [](ANativeActivity* activity) {
    return ACT->OnResume();
  };
  activity->callbacks->onPause = [](ANativeActivity* activity) {
    return ACT->OnPause();
  };
  activity->callbacks->onStop = [](ANativeActivity* activity) {
    return ACT->OnStop();
  };
  activity->callbacks->onDestroy = [](ANativeActivity* activity) {
    delete ACT;
    activity->instance = nullptr;
  };
  activity->callbacks->onNativeWindowCreated = [](ANativeActivity* activity,
                                                  ANativeWindow* window) {
    ACT->OnNativeWindowCreated(window);
  };
  activity->callbacks->onNativeWindowResized = [](ANativeActivity* activity,
                                                  ANativeWindow* window) {
    ACT->OnNativeWindowResize(window);
  };
  activity->callbacks->onNativeWindowDestroyed = [](ANativeActivity* activity,
                                                    ANativeWindow* window) {
    ACT->OnNativeWindowDestroyed(window);
  };
  activity->callbacks->onLowMemory = [](ANativeActivity* activity) {
    return ACT->OnLowMemory();
  };
  return true;
#undef ACT
}

bool Activity::StartEngine() {
  auto engine = std::make_unique<Engine>();
  if (!engine->Launch()) {
    return false;
  }
  UpdateEngineSurfaceSize();
  engine_ = std::move(engine);
  return true;
}

bool Activity::StopEngine() {
  engine_.reset();
  return true;
}

void Activity::UpdateEngineSurfaceSize() {
  if (surfaces_.empty() || !engine_) {
    return;
  }
  auto window = surfaces_.begin()->first;
  const auto width = std::max(0, ANativeWindow_getWidth(window));
  const auto height = std::max(0, ANativeWindow_getHeight(window));
  engine_->SetSurfaceSize(FlutterUIntSize{
      .width = width,
      .height = height,
  });
}

}  // namespace embedder
