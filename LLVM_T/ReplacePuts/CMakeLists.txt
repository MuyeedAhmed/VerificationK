cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(PrintfToPuts)

find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
add_executable(dummy EXCLUDE_FROM_ALL main.cpp)

add_executable(PrintfToPuts main.cpp)
target_link_libraries(PrintfToPuts
  clangTooling
  clangBasic
  clangASTMatchers
)
