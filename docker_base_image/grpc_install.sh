set -ex

git clone -b master --recursive https://github.com/grpc/grpc
cd grpc

git checkout 42284424acd2ba4ba649599984592f5d2eade919

git submodule update --init

mkdir -p cmake/build
cd cmake/build
cmake ../.. -DBUILD_SHARED_LIBS=ON  -DCMAKE_BUILD_TYPE=Release 
make
sudo make install
