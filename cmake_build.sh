#!/bin/bash

mkdir -p build
cd build
cmake .. -DBUILD_WITH_AMENT=OFF -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
make -j$(nproc)

# mkdir -p build
# cd build
# cmake .. \
#   -DBUILD_WITH_AMENT=OFF \
#   -DCMAKE_INSTALL_PREFIX=../install \
#   -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# make -j$(nproc)
# make install