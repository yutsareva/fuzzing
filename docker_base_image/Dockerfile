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

COPY grpc_install.sh /
RUN bash /grpc_install.sh

COPY libprotobuf_mutator_install.sh /
RUN bash /libprotobuf_mutator_install.sh

COPY aflplusplus_install.sh /
RUN bash /aflplusplus_install.sh

