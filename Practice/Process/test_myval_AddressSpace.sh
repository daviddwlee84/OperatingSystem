#!/bin/bash

gcc myval.c -o myval.out

./myval.out 5 > myval_1.log &
MYVAL_1_PID=$!
./myval.out 6 > myval_2.log &
MYVAL_2_PID=$!

# How to run a process with a timeout in Bash?
# https://stackoverflow.com/questions/10224939/how-to-run-a-process-with-a-timeout-in-bash
sleep 1
kill $MYVAL_1_PID
kill $MYVAL_2_PID

echo "This is myval with argument 5"
head -5 myval_1.log

echo "This is myval with argument 6"
head -5 myval_2.log

echo "They have \"same address\" with different value."
echo "==> this address is in \"Virtual Address Space\""
# The result seems different from slides of PKU course = =

rm myval.out myval_1.log myval_2.log
