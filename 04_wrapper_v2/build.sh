#!/bin/bash

set -ex

cp -r ../grpc_server .

docker build -t 04-wrapper .
