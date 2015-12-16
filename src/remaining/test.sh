#!/bin/bash


PARSER="-parser"
SEMANTIC="-semantic"
OPTIMIZATION="-optimization"
BINARY="-binary"
CODE="-code"
FLAGS="$PARSER,$SEMANTIC,$OPTIMIZATION,$BINARY,$CODE"

TEST_PATH="../testpgm/"

gcc -c diesel_rts.c -o diesel_rts.o -Wall -m64
make

if [ "$1" == "$PARSER" ]
then
    ./diesel -a -s -c -f -p ""$TEST_PATH"parstest1.d"
    ./diesel -a -s -c -f -p ""$TEST_PATH"parstest2.d"
    ./diesel -a -s -c -f -p ""$TEST_PATH"parstest3.d"
elif [ "$1" == "$SEMANTIC" ]
then
    ./diesel -s -p -f -y -a ""$TEST_PATH"semtest1.d"
    ./diesel -s -p -f -y -a ""$TEST_PATH"semtest2.d"
elif [ "$1" == "$OPTIMIZATION" ]
then
    ./diesel -s -p -a ""$TEST_PATH"opttest1.d"
elif [ "$1" == "$BINARY" ]
then
    ./diesel -s -q -y -a ""$TEST_PATH"quadtest1.d"
elif [ "$1" == "$CODE" ]
then
    ./diesel -q -a -t -y  ""$TEST_PATH"codetest1.d"
else
    echo "Invalid or no flag set, use either of following flags: $FLAGS"
fi


exit 0
