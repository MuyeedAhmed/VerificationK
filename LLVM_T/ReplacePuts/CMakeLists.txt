cmake_minimum_required(VERSION 3.16)

# Enable both C and C++ languages
project(PrintfToPuts LANGUAGES C CXX)

# Enable compile_commands.json generation
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find LLVM and Clang packages
find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

message(STATUS "Found LLVM: ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Include Clang and LLVM directories explicitly
include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})
include_directories(/usr/lib/llvm-16/include)  # Ensure this points to your LLVM installation

link_directories(${LLVM_LIBRARY_DIRS})

# Add executable
add_executable(PrintfToPuts main.cpp)

# Link necessary libraries
target_link_libraries(PrintfToPuts
    clangTooling
    clangBasic
    clangASTMatchers
    LLVM
)
