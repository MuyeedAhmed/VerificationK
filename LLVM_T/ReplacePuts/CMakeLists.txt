cmake_minimum_required(VERSION 3.16)

# Enable both C and C++ languages
project(PrintfToPuts LANGUAGES C CXX)

# Enable compile_commands.json generation
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include LLVM and Clang
find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

message(STATUS "Found LLVM: ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Include directories
include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})

# Add the executable
add_executable(PrintfToPuts main.cpp)

# Link the required LLVM/Clang libraries
target_link_libraries(PrintfToPuts
    clangTooling
    clangBasic
    clangASTMatchers
    LLVM
)
