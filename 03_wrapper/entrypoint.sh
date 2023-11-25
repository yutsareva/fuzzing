#!/bin/sh

AFL_DISABLE_TRIM=1 AFL_CUSTOM_MUTATOR_ONLY=1 AFL_CUSTOM_MUTATOR_LIBRARY=/custom_mutator.so AFL_SKIP_CPUFREQ=1 /AFLplusplus/afl-fuzz -i ./in -o ./out -t 1100 -- /exmpl-cmake-grpc/build/server/server

# Keep the container running
tail -f /dev/null
