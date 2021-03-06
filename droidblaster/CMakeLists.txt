# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

set(${CMAKE_C_FLAGS}, "${CMAKE_C_FLAGS}")
add_definitions("-DANDROID_NDK -DDISABLE_IMPORTGL")

add_library(droidblaster
          SHARED
          src/main/cpp/Main.cpp)

add_library(blaster_log
         STATIC
         src/main/cpp/Log.cpp)

add_library(event_loop
         STATIC
         src/main/cpp/EventLoop.cpp)

add_library(native_app_glue
          STATIC
          ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

add_library(droid_blaster
          STATIC
          src/main/cpp/DroidBlaster.cpp)

add_library(graphics_manager
            STATIC
            src/main/cpp/GraphicsManager.cpp)

add_library(ship
            STATIC
            src/main/cpp/ship.cpp)

# 指定头文件 .h 路径
# Specifies a path to native header files.
include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${ANDROID_NDK}/sources/android/native_app_glue/)


# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       droidblaster
                       droid_blaster
                       ship
                       graphics_manager
                       event_loop
                       blaster_log
                       native_app_glue
                       android
                       log

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )