# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

set(PROJECT_NAME native-lib)
project(${PROJECT_NAME} CXX)
set(CMAKE_CXX_STANDARD 14 CACHE STRING "The C++ standard whose features are requested to build this target.")
set(CMAKE_VERBOSE_MAKEFILE ON)

# Build the main target `${PROJECT_NAME}` that will use TF Lite
add_library( ${PROJECT_NAME} SHARED native-lib.cpp )


# opencv
set(OpenCV_STATIC ON)
set(OpenCV_DIR /DATA/android/android_packages/OpenCV-android-sdk/sdk/native/jni/)
find_package(OpenCV REQUIRED)

## LibTorch
#set(path_libtorch /home/sjyoon1671/Desktop/TF-Lite-Cpp-API-Android-Example/app/src/main/ml/libtorch/cpu)
#set(LIBRARY_PATH ${LIBRARY_PATH}:${path_libtorch}/lib)
#set(LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:${path_libtorch}/lib)
#set(Torch_DIR ${path_libtorch})
#
## Library: Torch
#find_package(Torch REQUIRED)
#file(GLOB PYTORCH_INCLUDE_DIRS "${build_DIR}/pytorch_android*.aar/headers")
#file(GLOB PYTORCH_LINK_DIRS "${build_DIR}/pytorch_android*.aar/jni/${ANDROID_ABI}")
#
#set(BUILD_SUBDIR ${ANDROID_ABI})
#
#find_library(PYTORCH_LIBRARY pytorch_jni
#        PATHS ${PYTORCH_LINK_DIRS}
#        NO_CMAKE_FIND_ROOT_PATH)
#find_library(FBJNI_LIBRARY fbjni
#        PATHS ${PYTORCH_LINK_DIRS}
#        NO_CMAKE_FIND_ROOT_PATH)
#include_directories(${PYTORCH_INCLUDE_DIRS})
#
#
# Relative path of gradle build directory to CMakeLists.txt


#set(build_DIR ${CMAKE_SOURCE_DIR}/../../build)
set(build_DIR /home/sjyoon1671/Desktop/Android/TF-Lite-Cpp-API-Android-Example/app/build)
#
#
file(GLOB PYTORCH_INCLUDE_DIRS "${build_DIR}/pytorch_android*.aar/headers")
file(GLOB PYTORCH_LINK_DIRS "${build_DIR}/pytorch_android*.aar/jni/${ANDROID_ABI}")


#
#
set(BUILD_SUBDIR ${ANDROID_ABI})
target_include_directories(${PROJECT_NAME} PRIVATE ${PYTORCH_INCLUDE_DIRS})
#
#
find_library(PYTORCH_LIBRARY pytorch_jni
        PATHS ${PYTORCH_LINK_DIRS}
        NO_CMAKE_FIND_ROOT_PATH)
#
#
#find_library(FBJNI_LIBRARY fbjni
#        PATHS ${PYTORCH_LINK_DIRS}
#        NO_CMAKE_FIND_ROOT_PATH)
##
##



#
#
# Specify where to find the header files for TF Lite C++
set( INCLUDE_DIRS
        ${CMAKE_CURRENT_LIST_DIR}/tf-lite-api/include
        ${CMAKE_CURRENT_LIST_DIR}/tf-lite-api/include/abseil
        ${CMAKE_CURRENT_LIST_DIR}/tf-lite-api/include/flatbuffers/include)
include_directories(${INCLUDE_DIRS})

add_library( tflite SHARED IMPORTED )
set_target_properties( tflite PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/tf-lite-api/generated-libs/${ANDROID_ABI}/libtensorflowlite.so ) # libtensorflowlite, libtensorflowlite_gpu_delegate.so

add_library( tflite_gpu SHARED IMPORTED )
set_target_properties( tflite_gpu PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/tf-lite-api/generated-libs/${ANDROID_ABI}/libtensorflowlite_gpu_delegate.so ) # libtensorflowlite, libtensorflowlite_gpu_delegate.so

# Ori


find_library( log-lib log ) # Library required by NDK.
find_library(android-lib android) # for AssetManager functionality


#include_directories(${CMAKE_CURRENT_SOURCE_DIR}
#        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c )

# Link the main target with two required libs: `log` and `libtensorflowlite.so`
# target_link_libraries( ${PROJECT_NAME} ${log-lib} ${android-lib} ${OpenCV_LIBS} tflite)
# target_link_libraries( ${PROJECT_NAME} ${log-lib} ${android-lib} ${OpenCV_LIBS} tflite tflite_gpu)
target_link_libraries( ${PROJECT_NAME} ${log-lib} ${android-lib} ${OpenCV_LIBS} tflite tflite_gpu ${PYTORCH_LIBRARY}) # ${FBJNI_LIBRARY}
#target_link_libraries( ${PROJECT_NAME} app-glue ${log-lib} ${android-lib} ${OpenCV_LIBS} tflite tflite_gpu ${PYTORCH_LIBRARY}) # ${FBJNI_LIBRARY}


# sed -i '/# Android logcat for NDK/d' ./Android.mk
