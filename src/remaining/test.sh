#!/bin/bash

FLAGS="-parser,-semantic"

make

if [ "$1" == "-parser" ]
then
    ./diesel -a -b -c -f -p ../testpgm/parstest1.d
    ./diesel -a -b -c -f -p ../testpgm/parstest2.d
    ./diesel -a -b -c -f -p ../testpgm/parstest3.d
elif [ "$1" == "-semantic" ]
then
    ./diesel -b -p -f -y -a ../testpgm/semtest1.d
    ./diesel -b -p -f -y -a ../testpgm/semtest2.d
else
    echo "Invalid or no flag set, use either of following flags: $FLAGS"
fi


exit 0
