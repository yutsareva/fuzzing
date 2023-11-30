set(AFL_CC_COMPILER LLVM)

set(CMAKE_C_COMPILER /AFLplusplus/afl-clang-fast)
set(CMAKE_CXX_COMPILER /AFLplusplus/afl-clang-fast++)

set(ENV{CC} /AFLplusplus/afl-clang-fast)
set(ENV{CXX} /AFLplusplus/afl-clang-fast++)

cmake_minimum_required(VERSION 3.13)
project(example-grpc LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(proto)
