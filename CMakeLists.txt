cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

project(Grime VERSION 1.0)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin/)

file(GLOB_RECURSE SRC_FILES src/*.cpp vendor/src/*.cpp)

add_compile_options(-Wall -Wextra -Wpedantic)
add_executable(Grime ${SRC_FILES})
target_include_directories(Grime PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/vendor/include ${CMAKE_CURRENT_SOURCE_DIR}/vendor/include/imgui)
TARGET_LINK_LIBRARIES(Grime ${CMAKE_CURRENT_SOURCE_DIR}/vendor/lib/libglfw3.a X11 GL)

