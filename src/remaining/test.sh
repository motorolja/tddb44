#!/bin/bash


PARSER="-parser"
SEMANTIC="-semantic"
OPTIMIZATION="-optimization"
BINARY="-binary"
FLAGS="$PARSER,$SEMANTIC,$OPTIMIZATION,$BINARY"

TEST_PATH="../testpgm/"

make

if [ "$1" == "$PARSER" ]
then
    ./diesel -a -b -c -f -p ""$TEST_PATH"parstest1.d"
    ./diesel -a -b -c -f -p ""$TEST_PATH"parstest2.d"
    ./diesel -a -b -c -f -p ""$TEST_PATH"parstest3.d"
elif [ "$1" == "$SEMANTIC" ]
then
    ./diesel -b -p -f -y -a ""$TEST_PATH"semtest1.d"
    ./diesel -b -p -f -y -a ""$TEST_PATH"semtest2.d"
elif [ "$1" == "$OPTIMIZATION" ]
then
    ./diesel -b -p -a ""$TEST_PATH"opttest1.d"
elif [ "$1" == "$BINARY" ]
then
    ./diesel -b -q -y ""$TEST_PATH"quadtest1.d"
else
    echo "Invalid or no flag set, use either of following flags: $FLAGS"
fi


exit 0
