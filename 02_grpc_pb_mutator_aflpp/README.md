Test setup:
- llvm-14
- afl++
- simple grpc server with injected crash
- libprotobuf-mutator & afl++ custom mutator
- custom afl++ send implementation


## How to run


### Build docker image

```bash
docker build -t grpc-pb-mutator-aflpp-fuzz . 
```


### How to collect corpus

Alter exmpl-cmake-grpc/client/src/main.cpp to enable saving of protobuf messages to a file. Use these files as input samples for the fuzzer.


### Launch fuzzing

```bash
docker run -it grpc-pb-mutator-aflpp-fuzz bash
# inside container
AFL_DISABLE_TRIM=1 AFL_CUSTOM_MUTATOR_ONLY=1 AFL_CUSTOM_MUTATOR_LIBRARY=/custom_mutator.so AFL_SKIP_CPUFREQ=1 /AFLplusplus/afl-fuzz -i ./in -o ./out -t 1100 -- /exmpl-cmake-grpc/build/server/server
```
