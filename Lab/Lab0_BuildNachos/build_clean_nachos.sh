#!/bin/bash
# Copy from local
cp ../../Files/nachos-3.4.tar.gz .
tar xzf nachos-3.4.tar.gz
mv nachos_dianti nachos
rm -r nachos/gnu-decstation-ultrix/arm

# Build docker
docker build --tag nachos:0.0.1 --file original.Dockerfile .

# Clean up
rm -r nachos-3.4.tar.gz nachos