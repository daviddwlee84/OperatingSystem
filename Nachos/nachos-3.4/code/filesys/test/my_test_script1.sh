#!/bin/sh
# goto filesys/ in docker
cd /nachos/nachos-3.4/code/filesys

./nachos -f # format the DISK
./nachos -cp test/big big # copies a file from UNIX to Nachos
# Files: big
./nachos -cp test/small small
# Files: big, small
./nachos -l # lists the contents of the Nachos directory
# big
# small
./nachos -r big # remove the file "big" from Nachos
./nachos -D # prints the contents of the entire file system 
./nachos -t # tests the performance of the Nachos file system
