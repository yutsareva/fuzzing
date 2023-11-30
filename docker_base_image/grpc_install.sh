git clone -b master --recursive https://github.com/grpc/grpc
cd grpc

git checkout 42284424acd2ba4ba649599984592f5d2eade919

git submodule update --init

#cd third_party/protobuf
#./autogen.sh
#./configure
#make
#make check
#sudo make install
#sudo ldconfig # refresh shared library cache.

#cd ../.. # Back to grpc directory
#make
#sudo make install


mkdir -p cmake/build
cd cmake/build
cmake ../.. -DBUILD_SHARED_LIBS=ON
make
make install

