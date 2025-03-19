#!/bin/bash -e

# Entering correct directory
cd $(dirname "$0")
BASEDIR=$(pwd)

mkdir -p build_release
cd build_release
mkdir -p SECoreBin
mkdir -p SBSFU
mkdir -p Karmeliet
mkdir -p Binary
mkdir -p Binary/manufacturing
cp ../cmake-tools/flash.sh Binary/manufacturing/.