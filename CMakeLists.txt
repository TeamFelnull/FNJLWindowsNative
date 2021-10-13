cmake_minimum_required(VERSION 3.20)
project(FNJLWindowsNative)

set(CMAKE_CXX_STANDARD 14)

add_library(FNJLWindowsNative SHARED dev_felnull_fnjl_jni_windows_WindowsNative.cpp)
