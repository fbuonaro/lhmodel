#!/bin/bash

DOCKERFILE_PATH="./Dockerfiles/Dockerfile.swagger-codegen"
BUILD_STAGE="lhmodel-scg-stage-build-env"
BUILD_TAG="lhmodel-scg:build"
TARGET_STAGE="lhmodel-scg-s2-build"
TARGET_TAG="lhmodel-scg:main"
BASE_STAGE="lhmodel-scg-stage-base-env"

# Build the main image
docker build --target ${TARGET_STAGE} -t ${TARGET_TAG} -f ${DOCKERFILE_PATH} . || exit 1
