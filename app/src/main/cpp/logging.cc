#include "logging.h"

#include <android/log.h>

namespace embedder {

static android_LogPriority ToAndroidLogPriority(LogLevel level) {
  switch (level) {
    case LogLevel::kInfo:
      return ANDROID_LOG_INFO;
    case LogLevel::kWarn:
      return ANDROID_LOG_WARN;
    case LogLevel::kError:
      return ANDROID_LOG_ERROR;
    case LogLevel::kFatal:
      return ANDROID_LOG_FATAL;
  }
}

static void EmitLog(LogLevel level, const char* message) {
  __android_log_write(ToAndroidLogPriority(level), "flutter_embedder", message);
}

Logger::Logger(LogLevel level, const char* file, int line)
    : level_(level), file_(file), line_(line) {
  stream_ << file << ":" << line << ": ";
}

Logger::~Logger() {
  EmitLog(level_, stream_.str().c_str());
}

}  // namespace embedder
