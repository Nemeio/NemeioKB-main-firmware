#!/bin/bash

set -ex

MAX_VERSION_STR_LEN=16

generate_env() {
	local PREFIX=$1
	local MAJOR_NUMBER=$2
	local BUILD_NUMBER=$3

	if [ "x${MAJOR_NUMBER}" == "x" ]; then
	    exit 1
	fi

	if [ "x${BUILD_NUMBER}" == "x" ]; then
	    BUILD_NUMBER=00
	fi

	if [ $MAJOR_NUMBER -ge 2048 ] || [ $BUILD_NUMBER -ge 32 ]; then
		exit 1;
	fi

	echo "Generating versioning for ${PREFIX} with major ${MAJOR_NUMBER} and build number ${BUILD_NUMBER}"

	local STRING_VERSION_NUMBER=$MAJOR_NUMBER.$BUILD_NUMBER
	local BINARY_VERSION_NUMBER=$((${MAJOR_NUMBER#0}*32+${BUILD_NUMBER#0}))

	if (( ${#STRING_VERSION_NUMBER} > $MAX_VERSION_STR_LEN-1 )); then
		echo "Version number cannot contain more than "$MAX_VERSION_STR_LEN" characters"
		unset STRING_VERSION_NUMBER
		unset BINARY_VERSION_NUMBER
		exit 1
	fi

	eval "export ${PREFIX}_STRING_VERSION_NUMBER=${STRING_VERSION_NUMBER}"
	eval "export ${PREFIX}_BINARY_VERSION_NUMBER=${BINARY_VERSION_NUMBER}"
	eval "export ${PREFIX}_MAJOR_NUMBER=${MAJOR_NUMBER#0}"
	eval "export ${PREFIX}_BUILD_NUMBER=${BUILD_NUMBER#0}"
}

source version.txt

generate_env "APP" ${APP_MAJOR_NUMBER} ${APP_BUILD_NUMBER}
