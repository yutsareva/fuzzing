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

# COPY . /

COPY grpc_install.sh /
RUN bash /grpc_install.sh

COPY libprotobuf_mutator_install.sh /
RUN bash /libprotobuf_mutator_install.sh

COPY aflplusplus_install.sh /
RUN bash /aflplusplus_install.sh


RUN git clone https://github.com/faaxm/exmpl-cmake-grpc.git
COPY exmpl-cmake-grpc.patch /
RUN cd exmpl-cmake-grpc \
    && git apply /exmpl-cmake-grpc.patch \
    && mkdir build && cd build \
    && cmake .. \
    && make

RUN apt-get install vim -y

RUN ln -s $(which grpc_cpp_plugin) /usr/bin/protoc-gen-grpc && \
    protoc --proto_path=/exmpl-cmake-grpc/proto  --cpp_out=. --grpc_out=. myproto/address.proto myproto/addressbook.proto

COPY custom_mutator.cpp /
COPY Makefile /
RUN make 

COPY in /in
COPY entrypoint.sh /
ENTRYPOINT ["bash", "/entrypoint.sh"]
