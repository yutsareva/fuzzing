#!/bin/bash

set -ex

docker build -t  dockertsareva/fuzzing-base-image .
docker push dockertsareva/fuzzing-base-image
