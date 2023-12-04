set -ex

sudo apt-get update
sudo apt-get install -y build-essential python3-dev automake cmake git flex bison libglib2.0-dev libpixman-1-dev python3-setuptools cargo libgtk-3-dev

sudo apt-get install -y lld-14 llvm-14 llvm-14-dev clang-14
sudo apt-get install -y gcc-$(gcc --version|head -n1|sed 's/\..*//'|sed 's/.* //')-plugin-dev libstdc++-$(gcc --version|head -n1|sed 's/\..*//'|sed 's/.* //')-dev
sudo apt-get install -y ninja-build

git clone https://github.com/AFLplusplus/AFLplusplus
cd AFLplusplus

export LLVM_CONFIG=llvm-config-14

git checkout 61e27c6b54f7641a168b6acc6ecffb1754c10918

make
sudo make install
