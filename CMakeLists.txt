cmake_minimum_required(VERSION 3.20)
project(FNJLWindowsNative)

set(CMAKE_CXX_STANDARD 14)

add_library(FNJLWindowsNative SHARED dev_felnull_fnjl_jni_windows_WindowsNative.cpp)

set_target_properties(FNJLWindowsNative PROPERTIES PREFIX "")
set_target_properties(FNJLWindowsNative PROPERTIES OUTPUT_NAME "FNJLNative")
set_target_properties(FNJLWindowsNative PROPERTIES SUFFIX ".dll")

