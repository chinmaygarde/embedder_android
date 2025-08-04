#pragma once

#include <sstream>
#include <string>
#include "macros.h"

namespace embedder {

enum class LogLevel {
  kInfo,
  kWarn,
  kError,
  kFatal,
};

class Logger {
 public:
  Logger(LogLevel level, const char* file, int line);

  ~Logger();

  std::ostream& stream() { return stream_; }

 private:
  const LogLevel level_;
  const char* file_ = nullptr;
  const int line_;
  std::ostringstream stream_;

  EM_DISALLOW_COPY_ASSIGN_AND_MOVE(Logger);
};

#define LOG_INFO \
  ::embedder::Logger{::embedder::LogLevel::kInfo, __FILE__, __LINE__}.stream()
#define LOG_WARN \
  ::embedder::Logger{::embedder::LogLevel::kWarn, __FILE__, __LINE__}.stream()
#define LOG_ERROR \
  ::embedder::Logger{::embedder::LogLevel::kError, __FILE__, __LINE__}.stream()
#define LOG_FATAL \
  ::embedder::Logger{::embedder::LogLevel::kFatal, __FILE__, __LINE__}.stream()

}  // namespace embedder
