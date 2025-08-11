cmake_minimum_required(VERSION 3.22.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD              99)
set(CMAKE_CXX_STANDARD            20)

cmake_policy(SET CMP0135 NEW)
include(FetchContent)

# Enable CCache if available.
find_program(CCACHE_PROGRAM NAMES ccache)
if(CCACHE_PROGRAM)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CCACHE_PROGRAM}")
endif()

project("embedder")

set(FLUTTER_ENGINE_SHA 37736df6710b92036f28dd72203365725245740e)

FetchContent_Declare(
  flutter_embedder_sdk
  URL https://storage.googleapis.com/flutter_infra_release/flutter/${FLUTTER_ENGINE_SHA}/android-arm64/android-arm64-embedder.zip
)
FetchContent_MakeAvailable(flutter_embedder_sdk)

add_library(flutter_embedder_sdk SHARED IMPORTED)

set_target_properties(flutter_embedder_sdk
  PROPERTIES
  IMPORTED_LOCATION ${flutter_embedder_sdk_SOURCE_DIR}/libflutter_engine.so
)
target_include_directories(flutter_embedder_sdk
  INTERFACE
  ${flutter_embedder_sdk_SOURCE_DIR}
)

message(STATUS "Downloaded Flutter Embedder SDK at version https://github.com/flutter/flutter/commit/${FLUTTER_ENGINE_SHA}")

# This must match the value in android.app.lib_name in AndroidManifest.xml
add_library(embedder SHARED
  embedder.cc
  engine.cc
  engine.h
  logging.cc
  logging.h
  macros.h
)

target_link_libraries(embedder
  android
  log
  flutter_embedder_sdk
)
