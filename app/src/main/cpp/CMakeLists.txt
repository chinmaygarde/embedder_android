cmake_minimum_required(VERSION 3.22.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project("embedder")

add_library(${CMAKE_PROJECT_NAME} SHARED
  embedder.cc
)

target_link_libraries(${CMAKE_PROJECT_NAME}
  android
  log
)
