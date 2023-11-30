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
04_wrapper_v2 $ bash build.sh
04_wrapper_v2 $ docker run 04-wrapper
```


### Launch fuzzing

```bash
docker run 04-wrapper
```

### How to reproduce crash

```bash
cat /out/default/crashes/<crash_id> | /grpc_server/build/server/server
```
