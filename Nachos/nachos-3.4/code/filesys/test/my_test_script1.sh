#!/bin/sh
# goto filesys/ in docker
cd /nachos/nachos-3.4/code/filesys

# use -Q to disable verbose machine messages
echo "=== format the DISK ==="
./nachos -Q -f
echo "=== copies file \"big\" from UNIX to Nachos ==="
./nachos -Q -cp test/big big
# Files: big
echo "=== copies file \"small\" from UNIX to Nachos ==="
./nachos -Q -cp test/small small
# Files: big, small
echo "=== lists the contents of the Nachos directory ==="
./nachos -Q -l
# big
# small
echo "=== remove the file \"big\" from Nachos ==="
./nachos -Q -r big
echo "=== print the content of file \"small\" ==="
./nachos -Q -p small
echo "=== prints the contents of the entire file system ==="
./nachos -Q -D
echo "=== tests the performance of the Nachos file system ==="
./nachos -Q -t
