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

echo Step 4: Create Makefile by CMake. Makefile is located at ./build
rm -rf ./build
mkdir ./build
cd ./build
export CXXFLAGS="-I${NAN_INCLUDE_PATH} -I${NODE_INCLUDE_PATH}"
cmake -DSPDLOG_LOG_LEVEL_COMPILE=SPDLOG_LEVEL_INFO ../
echo All done. Now really perform builds by command:
echo 'cd ./build/ && make'
