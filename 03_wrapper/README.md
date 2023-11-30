Test setup:
- llvm-14
- afl++
- simple grpc server with injected crash
- libprotobuf-mutator & afl++ custom mutator
- wrapper to make gRPC calls
    - supports multiple calls in a single test


## How to run


### Build docker image

```bash
docker build -t 03_wrapper . 
```


### How to collect corpus

Alter exmpl-cmake-grpc/client/src/main.cpp to enable saving of protobuf messages to a file. Use these files as input samples for the fuzzer.

See `client_save_corpus.patch`.


### Launch fuzzing

```bash
docker run 03_wrapper
```

### How to reproduce crash

```bash
cat /out/default/crashes/<crash_id> | /exmpl-cmake-grpc/build/server/server
```
