#!/bin/bash

#LZMA
cd ..
mkdir -p libs
cd libs
curl -sSL https://raw.githubusercontent.com/elsamuko/xz_test/master/scripts/build_lzma_linux.sh | bash

#SODIUM
wget https://download.libsodium.org/libsodium/releases/libsodium-1.0.18-stable.tar.gz
tar xzf libsodium-1.0.18-stable.tar.gz
rm libsodium-1.0.18-stable.tar.gz
cd libsodium-stable
./configure
make -j4 && make check

cd ..
cd ..
rm -rf tmp