#!/bin/bash


PARSER="-parser"
SEMANTIC="-semantic"
OPTIMIZATION="-optimization"
BINARY="-binary"
CODE="-code"
ALL="-all"
FLAGS="$PARSER,$SEMANTIC,$OPTIMIZATION,$BINARY,$CODE,$ALL"
TEST_FILES=("codetest1" "quadtest1" "8q")

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
elif [ "$1" == "$ALL" ]
then

for test_file  in ${TEST_FILES[*]}
do
    echo "compiling: $test_file"
    ./diesel -o $test_file".o" ""$TEST_PATH""$test_file".d"
    echo "execute: $test_file "
    ./$test_file".o"
done
else
    echo "Invalid or no flag set, use either of following flags: $FLAGS"
fi


exit 0
