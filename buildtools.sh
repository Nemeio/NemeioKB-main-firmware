#!/bin/bash -e

NPROC=`nproc`

TOOLS_DIRECTORIES="Tools/QSPI/FirmwareEnvGenerator Tools/QSPI/PrepareImage Tools/QSPI/LfsGenerator"

# Entering correct directory
cd $(dirname "$0")
BASEDIR=$(pwd)

mkdir -p build_release/Tools

for dir in ${TOOLS_DIRECTORIES}; do
	program_name=$(basename -- $dir)
	cd ${BASEDIR}/$dir
	mkdir -p build
	cd build
	cmake ..
	make -j8
	cp ${program_name} ${BASEDIR}/build_release/Tools/.
done

