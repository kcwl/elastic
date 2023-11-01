#!/bin/bash

apt-get install libboost-all-dev -y

apt-get install git -y

git clone https://github.com/protocolbuffers/protobuf.git

git switch -C v21.12

cmake .

make -j8

make install