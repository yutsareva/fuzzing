FROM ubuntu:22.04

RUN apt-get -y update && \
    apt-get -y install sudo \ 
    apt-utils \
    build-essential \
    openssl \
    clang-12 \
    graphviz-dev \
    libcap-dev \
    git \
    cmake \
    tcpdump

RUN apt-get install -y curl zip unzip tar libpqxx-dev && \
    git clone https://github.com/microsoft/vcpkg && \
  ./vcpkg/bootstrap-vcpkg.sh  && \
  ./vcpkg/vcpkg install "protobuf" --recurse  && \
  ./vcpkg/vcpkg install "grpc" --recurse

RUN git clone https://github.com/faaxm/exmpl-cmake-grpc.git

# Download and compile AFLNet
ENV LLVM_CONFIG="llvm-config-12"

RUN git clone https://github.com/aflnet/aflnet.git /opt/aflnet

RUN sudo ln -s /usr/bin/clang-12 /usr/bin/clang
RUN sudo ln -s /usr/bin/clang++-12 /usr/bin/clang++

RUN cd /opt/aflnet && make clean all && \
    cd llvm_mode && \
    make

# Set up environment variables for AFLNet
ENV AFLNET="/opt/aflnet"
ENV PATH="${PATH}:${AFLNET}"
ENV AFL_PATH="${AFLNET}"
ENV AFL_I_DONT_CARE_ABOUT_MISSING_CRASHES=1 \
    AFL_SKIP_CPUFREQ=1

# Build simple grpc server
COPY exmpl-cmake-grpc.patch / # patch with injected crash
COPY corpus /corpus  # pre-collected
# Inject crash
RUN cd exmpl-cmake-grpc && git apply /exmpl-cmake-grpc.patch
RUN cd exmpl-cmake-grpc && mkdir build && cd build \
    && cmake -DCMAKE_TOOLCHAIN_FILE=/vcpkg/scripts/buildsystems/vcpkg.cmake .. \
    && make

