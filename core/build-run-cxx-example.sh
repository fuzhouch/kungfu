#!/bin/sh

INSTALL_TO=$HOME/.local/kungfu
mkdir -p ${INSTALL_TO}

echo Build examples...
pushd ./examples && rm -rf ./build && mkdir -p ./build && cd ./build && cmake .. && cmake --build . --verbose --config Release
popd

echo ==============================================================
echo Run example...
echo ==============================================================
export LD_LIBRARY_PATH=${INSTALL_TO}/lib
./examples/build/yijinjing-demo
