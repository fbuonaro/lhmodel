#!/bin/bash

DOCKERFILE_PATH="./Dockerfiles/Dockerfile.lhmodel"
MAIN_STAGE="lhmodel-s4-main-env"
MAIN_TAG="lhmodel:main"
BUILD_STAGE="lhmodel-s1-build-env"
BUILD_TAG="lhmodel:build-env"
TARGET_STAGE=${MAIN_STAGE}
TARGET_TAG=${MAIN_TAG}
BASE_STAGE="lhmodel-stage-base-env"

if [[ $1 = "-build" ]];
then
    TARGET_STAGE=${BUILD_STAGE}
    TARGET_TAG=${BUILD_TAG}
fi

# Build the main image
docker build --target ${TARGET_STAGE} -t ${TARGET_TAG} -f ${DOCKERFILE_PATH} . || exit 1
