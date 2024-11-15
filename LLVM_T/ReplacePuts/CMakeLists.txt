cmake_minimum_required(VERSION 3.16)  # Ensure CMake is recent enough
project(PrintfToPuts LANGUAGES CXX)

# Enable the generation of compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set C++17 standard (required for the tool)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include directories for LLVM and Clang
find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

message(STATUS "Found LLVM: ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})

# Add the executable
add_executable(PrintfToPuts main.cpp)

# Link against Clang and LLVM libraries
target_link_libraries(PrintfToPuts
    clangTooling
    clangBasic
    clangASTMatchers
    LLVM
)
