#!/bin/bash
# Copy from local
cp -r ../../Nachos nachos

echo "Building nachos docker for ${1:-threads}..."

# Build docker of subdirectory
docker build --build-arg subdir=${1:-threads} --tag nachos_${1:-threads}:latest --file subdir.Dockerfile .

# Clean up
rm -r nachos