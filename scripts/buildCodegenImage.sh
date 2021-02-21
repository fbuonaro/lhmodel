lhmodel-scg-stage-base-env#!/bin/bash

DOCKERFILE_PATH="./Dockerfiles/Dockerfile.swagger-codegen"
BUILD_STAGE="lhmodel-scg-stage-build-env"
BUILD_TAG="lhmodel-scg:build"
TARGET_STAGE="lhmodel-scg-s2-build"
TARGET_TAG="lhmodel-scg:main"
BASE_STAGE="lhmodel-scg-stage-base-env"

# Build the main image
docker build --target ${TARGET_STAGE} -t ${TARGET_TAG} -f ${DOCKERFILE_PATH} . || exit 1

# Tag the intermediate image, expecting it to be build-env for generating skeleton
BUILD_IMAGE=`docker image ls -a -f "label=${BUILD_STAGE}=yes" -q | head -1`
docker tag ${BUILD_IMAGE} ${BUILD_TAG} || exit 1

# Clean up overridden images
docker image prune -f --filter "label=${BASE_STAGE}" || exit 1
