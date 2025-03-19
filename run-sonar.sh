#!/bin/sh


# Bitbucket information must be given using environment variables
SONAR_TOKEN=$1
COVERAGE_DIR=$2

COMPILATION_DIR=build_tests/sonar
OUTPUT_DIR=work

if [ ! -d ${COVERAGE_DIR} ]; then
  exit 2
fi

mkdir -p ${COMPILATION_DIR} && cd ${COMPILATION_DIR}
mkdir -p ${OUTPUT_DIR}
cmake ../.. -DTOOLCHAIN_PATH=gcc-arm-none-eabi-10-2020-q4-major -DVERSION=1 -DAPP_MAJOR_NUMBER=1 -DAPP_BUILD_NUMBER=1

build-wrapper-linux-x86-64 --out-dir "$OUTPUT_DIR" make clean all -j$(nproc)

cd - || exit

if [ ! -z $BITBUCKET_PR_ID ]; then
  BITBUCKET_OPTS="-Dsonar.pullrequest.key=$BITBUCKET_PR_ID -Dsonar.pullrequest.branch=$BITBUCKET_BRANCH -Dsonar.pullrequest.base=$BITBUCKET_PR_DESTINATION_BRANCH"
else
  BITBUCKET_OPTS="-Dsonar.branch.name=$BITBUCKET_BRANCH"
fi


echo "============ Start analysis ============"
sonar-scanner -X \
  -Dsonar.cfamily.build-wrapper-output="${COMPILATION_DIR}/${OUTPUT_DIR}" \
  -Dsonar.sources=. \
  -Dsonar.organization=adeneo-embedded \
  -Dsonar.projectKey=adeneo-embedded_b1994-ldlc-karmeliet-firmware \
  -Dsonar.host.url=https://sonarcloud.io \
  -Dsonar.login=${SONAR_TOKEN} \
  ${BITBUCKET_OPTS} \
  -Dsonar.cfamily.threads=$(nproc) \
  -Dsonar.cfamily.cache.enabled=true \
  -Dsonar.cfamily.cache.path=$(pwd)/.sonar \
  -Dsonar.inclusions="App/**/*,Libraries/**/*,Middlewares/Nemeio/**/*,Src/**/*,Tools/**/*" \
  -Dsonar.exclusions="Drivers/**/*" \
  -Dsonar.verbose=false \
  -Dsonar.log.level=INFO \
  -Dsonar.scm.exclusions.disabled=true \
  -Dsonar.cfamily.gcov.reportsPath=${COVERAGE_DIR}
