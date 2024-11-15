cmake_minimum_required(VERSION 3.16)

# Project definition and languages
project(FooToBar LANGUAGES C CXX)

# Enable generation of compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find LLVM and Clang packages
find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

# Include necessary LLVM/Clang directories
include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})

# Add the main executable
add_executable(FooToBar main.cpp)

# Link against Clang and LLVM libraries
target_link_libraries(FooToBar
    clangTooling
    clangBasic
    clangASTMatchers
)

# Add Test.c as a dummy target to ensure it is included in compile_commands.json
add_executable(dummy_target Test.c)
set_target_properties(dummy_target PROPERTIES EXCLUDE_FROM_ALL TRUE)
