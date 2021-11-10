#!/bin/bash

DOCKERFILE_PATH="./Dockerfiles/Dockerfile.lhmodel"
MAIN_STAGE="lhmodel-s4-main-env"
MAIN_TAG="lhmodel:main"
TEST_STAGE="lhmodel-stage-test-env"
TEST_TAG="lhmodel:test-env"
TARGET_STAGE=${MAIN_STAGE}
TARGET_TAG=${MAIN_TAG}
BASE_STAGE="lhmodel-stage-base-env"

if [ $1 = "-test" ];
then
    TARGET_STAGE=${TEST_STAGE}
    TARGET_TAG=${TEST_TAG}
fi

# Build the main image
docker build --target ${TARGET_STAGE} -t ${TARGET_TAG} -f ${DOCKERFILE_PATH} . || exit 1
