#!/bin/sh
# goto filesys/ in docker
cd /nachos/nachos-3.4/code/filesys

# echo "Generate the large file for single indirect indexing"
# dd if=/dev/zero of=largeFile count=3 bs=1024 # 3KB

# echo "Generate the large file for double indirect indexing"
# dd if=/dev/urandom of=largeFile count=20 bs=1024 # 20KB

echo "Using 100,000 Decimal Digits of PI as large file"
cp test/PI.100.000.TXT largeFile # 112KB

# use -Q to disable verbose machine messages
echo "=== format the DISK ==="
./nachos -Q -f
echo "=== copies file \"largeFile\" from UNIX to Nachos ==="
./nachos -Q -cp largeFile largeFile
# ./nachos -Q -d f -cp largeFile largeFile # file debug
# ./nachos -Q -d d -cp largeFile largeFile # disk debug
# ./nachos -Q -d fd -cp largeFile largeFile # file and disk debug
echo "=== prints the contents of the entire file system ==="
./nachos -Q -D

echo "=== remove the file \"largeFile\" from Nachos ==="
./nachos -Q -r largeFile
echo "=== prints the contents of the entire file system again ==="
./nachos -Q -D
