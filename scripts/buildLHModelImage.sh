#!/bin/bash

DOCKERFILE_PATH="./Dockerfiles/Dockerfile.lhmodel"
TARGET_STAGE="lhmodel-s4-main-env"
TARGET_TAG="lhmodel:main"
TEST_STAGE="lhmodel-stage-test-env"
TEST_TAG="lhmodel:test-env"
BASE_STAGE="lhmodel-stage-base-env"

# Build the main image
docker build --target ${TARGET_STAGE} -t ${TARGET_TAG} -f ${DOCKERFILE_PATH} . || exit 1

# Tag the intermediate image, expecting it to be test-env
docker tag $(docker image ls -a -f "label=${TEST_STAGE}=yes" -q | head -1) ${TEST_TAG} || exit 1

# Clean up overridden images
docker image prune -f --filter "label=${BASE_STAGE}" || exit 1
