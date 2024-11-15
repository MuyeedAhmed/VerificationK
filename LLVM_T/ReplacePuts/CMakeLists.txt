cmake_minimum_required(VERSION 3.16)

# Enable both C and C++ languages
project(FooToBar LANGUAGES C CXX)

# Enable compile_commands.json generation
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find LLVM and Clang packages
find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})

# Add executable
add_executable(FooToBar main.cpp)

# Link necessary libraries
target_link_libraries(FooToBar
    clangTooling
    clangBasic
    clangASTMatchers
)
