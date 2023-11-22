FROM ubuntu:22.04

RUN apt-get -y update && \
    apt-get -y install sudo \ 
    apt-utils \
    build-essential \
    openssl \
    clang-14 \
    graphviz-dev \
    libcap-dev \
    git \
    cmake \
    tcpdump \
    autoconf automake libtool curl unzip \
    ninja-build \
    liblzma-dev \
    pip

RUN pip install --upgrade cmake

RUN sudo ln -s /usr/bin/clang-14 /usr/bin/clang
RUN sudo ln -s /usr/bin/clang++-14 /usr/bin/clang++

COPY . /

RUN bash /grpc_install.sh
RUN bash /libprotobuf_mutator_install.sh
RUN bash /aflplusplus_install.sh

RUN git clone https://github.com/faaxm/exmpl-cmake-grpc.git
RUN cd exmpl-cmake-grpc \
    && git apply /exmpl-cmake-grpc.patch \
    && mkdir build && cd build \
    && cmake .. \
    && make

RUN apt-get install vim -y

RUN ln -s $(which grpc_cpp_plugin) /usr/bin/protoc-gen-grpc && \
    protoc --proto_path=/exmpl-cmake-grpc/proto  --cpp_out=. --grpc_out=. myproto/address.proto myproto/addressbook.proto

RUN make 
# RUN AFL_DISABLE_TRIM=1 AFL_CUSTOM_MUTATOR_ONLY=1 AFL_CUSTOM_MUTATOR_LIBRARY=/custom_mutator.so AFL_SKIP_CPUFREQ=1 /AFLplusplus/afl-fuzz -i ./in -o ./out -t 1100 -- /exmpl-cmake-grpc/build/server/server
