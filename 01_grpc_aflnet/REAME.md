Test setup:
- llvm-12
- aflnet
- simple grpc server with injected crash


## How to run

### Build docker image
```bash
docker build -t grpc-aflnet-fuzz . 
docker run -it grpc-aflnet-fuzz bash
```

### Collect corpus [optional]
You can skip the step and use corpus/rtsp_raw.pcap file

```bash
# inside docker container

# 1. run server
/exmpl-cmake-grpc/build/server/server
# 2. run tcpdump
sudo tcpdump -w rtsp.pcap -i lo dst port 50051
# 3. run client
/exmpl-cmake-grpc/build/client/client
# 4. stop tcpdump
#    use Wireshark to extract only incoming packets with data https://github.com/aflnet/aflnet#step-1-prepare-message-sequences-as-seed-inputs
#    move the tcp stream file to /corpus folder
```
todo: use tshark instead of wireshark

### Launch fuzzing
```bash
# inside container
/opt/aflnet/afl-fuzz -d -i corpus -o /out-exmpl-grpc-fuzz -N tcp://127.0.0.1/50051 -D 10000 -P HTTP -q 3 -s 3 -K -R -m none /exmpl-cmake-grpc/build/server/server 

# wait for 10-20 mins
# results will be in out-exmpl-grpc-fuzz folder
ls /out-exmpl-grpc-fuzz/replayable-crashes

# reproduce crash
# 1. run server
docker exec -it $(docker ps -q) bash # new terminal
/exmpl-cmake-grpc/build/server/server 
# 2. replay crash (file name might differ)
/opt/aflnet/aflnet-replay /out-exmpl-grpc-fuzz/replayable-crashes/id:000000,sig:06,src:000011,op:havoc,rep:128 HTTP 50051
```
