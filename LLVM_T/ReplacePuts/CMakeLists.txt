cmake_minimum_required(VERSION 3.16)

# Enable both C and C++ languages
project(PrintfToPuts LANGUAGES C CXX)

# Forcefully enable compile_commands.json generation
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include LLVM and Clang
find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

message(STATUS "Found LLVM: ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Add the executable (ensure main.cpp exists)
add_executable(PrintfToPuts main.cpp)

# Link LLVM and Clang libraries
target_link_libraries(PrintfToPuts
    clangTooling
    clangBasic
    clangASTMatchers
    LLVM
)
