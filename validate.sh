#!/bin/bash

NPROC=`nproc`

# Entering correct directory
BASEDIR=$(dirname "$0")
cd ${BASEDIR}

echo "Validation"

echo "Get test samples"
git lfs pull --include "test/samples/**"

cd test
mkdir -p build
cd build

cmake ..
if ! make -j$NPROC
then
	exit 1
fi

if ! ./Karmeliet_test --gtest_output=xml:./test-reports/karmeliet_test_report.xml
then
	exit 1
fi

cd ../..

for gcda_file in `find ./test -name *.gcda`
do
    (cd `dirname ${gcda_file}` && gcov `basename ${gcda_file}`)
done

mkdir test-coverage
find test/ -name '*.gcov' -exec cp --parents {} test-coverage/ \;
