# Embedder Android

A example of using the [Flutter Embedder API](https://github.com/flutter/flutter/blob/master/engine/src/flutter/docs/Custom-Flutter-Engine-Embedders.md) on Android.

## Flutter Engine Selection

The Flutter Embedder API SDK is distributed as a prebuilt. This project downloads those prebuilts and implements the Embedder specific bits assuming no access official Flutter Android Java/Kotlin SDK.

The version of prebuilt Embedder API is specified in [`FLUTTER_ENGINE_SHA`](app/src/main/cpp/CMakeLists.txt).
