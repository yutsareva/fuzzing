#!/bin/bash

set -ex
export AFL_CC_COMPILER=LTO
export LD=afl-ld-lto
export AFL_LLVM_LAF_ALL=1
# export AFL_LLVM_ALLOWLIST=allowlist.txt
rm -rf build && mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Release  .. && make && cd ..
