cmake_minimum_required(VERSION 3.22.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD              99)
set(CMAKE_CXX_STANDARD            20)

# Enable CCache if available.
find_program(CCACHE_PROGRAM NAMES ccache)
if(CCACHE_PROGRAM)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CCACHE_PROGRAM}")
endif()

project("embedder")

# 37736df6710b92036f28dd72203365725245740e

# This must match the value in android.app.lib_name in AndroidManifest.xml
add_library(embedder SHARED
  embedder.cc
  embedder.h
  engine.cc
  engine.h
  logging.cc
  logging.h
  macros.h
)

target_link_libraries(embedder
  android
  log
)
