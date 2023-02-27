#!/bin/sh
# Before installation - Make sure you installed nvm script. We use nvm
# to install node v10.24.1, as this is the latest version used to build
# kungfu. In Archlinux or Manjaro, just use `pacman -S nvm`.
#
# Official project: https://github.com/nvm-sh/nvm 
#
# Command line:
#     nvm install 10.24.1 # Run once
#     nvm use 10.24.1     # Run every time from bash command line
#
# Potential problems:
# 1. Python binding can be unstable. Official documentation says it
#    compiles with Python 3 but didn't clarify which version it uses.
#    Assume a pretty old one. From built-in yarn build script it uses an
#    old pipenv version, which uses an unsupported --three command line
#    option. In my Manjaro test box there's only Python 3.10. There's no
#    way to downgrade. Thus the Python binding may have strange
#    behaviors.
#
# 2. Default yarn build script hardcodes Python227 and platform to
#    win32. In Linux the build always fail due to this. 

echo Step 0: Install node v10.24.1 with nvm
unset npm_config_prefix
source ${HOME}/.nvm/nvm.sh
nvm install 10.24.1

echo Step 1: Use node v10.24.1. On errors, stop and check nvm installation.
nvm use 10.24.1

echo Step 2: Install NodeJS native bindings.
NODE_VERSION=`node --version`
NODE_INCLUDE_PATH=${HOME}/.nvm/versions/node/${NODE_VERSION}/include/node
CXX_SRC_ROOT=`pwd`
npm install --save nan
NAN_INCLUDE_PATH=${CXX_SRC_ROOT}/node_modules/nan

echo Step 3: Fix googtest code that was treated as error in latest gcc. 
WARNING_FILE='deps/googletest-1.9.0/googletest/src/gtest-death-test.cc'
git checkout -- ${WARNING_FILE}
sed -i '1281s/int dummy;/int dummy = 0;/' ${WARNING_FILE}

echo Step 3.5: Detect python
PYTHON3_CONFIG=`which python3-config`

if [ "$PYTHON3_CONFIG" = "" ]; then
    echo ERROR: There is no python3-config detected.
    exit 1
fi

echo Step 4: Create Makefile by CMake. Makefile is located at ./build
rm -rf ./build
mkdir ./build
pushd ./build
INSTALL_TO=$HOME/.local/kungfu
export CXXFLAGS="-I${NAN_INCLUDE_PATH} -I${NODE_INCLUDE_PATH}"
echo Add CXXFLAGS: $CXXFLAGS
cmake -DFMT_INSTALL=1 \
      -DSPDLOG_LOG_LEVEL_COMPILE=SPDLOG_LEVEL_INFO \
      -DCMAKE_INSTALL_PREFIX=${INSTALL_TO} \
      -DPYBIND11_PYTHON_VERSION=3 \
      -DCMAKE_BUILD_TYPE=Release \
      ..
popd

echo Step 5: Install headers and libraries to ${INSTALL_TO}
pushd ./build && \
    cmake --build . --verbose --config Release && \
    make install
popd
