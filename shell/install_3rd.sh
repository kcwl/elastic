#!/bin/bash

apt-get install libboost-all-dev -y

apt-get install git -y

git clone --recurse-submodules https://github.com/protocolbuffers/protobuf.git

cd protobuf

git switch -C v21.12

cmake .

make -j8

make install