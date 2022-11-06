#!/bin/bash

TARGET_STAGE="$1"
if [[ "x${TARGET_STAGE}" == "x" ]];
then
    TARGET_STAGE="dist"
fi

./modules/lhscriptutil/scripts/buildImage.sh ./Dockerfiles/Dockerfile.lhmodel lhmodel "${TARGET_STAGE}"