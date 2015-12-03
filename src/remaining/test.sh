#!/bin/bash

./diesel -a -b -c -f -p ../testpgm/parstest1.d
./diesel -a -b -c -f -p ../testpgm/parstest2.d
./diesel -a -b -c -f -p ../testpgm/parstest3.d

#semantics

diesel -b -p -f -y -a ../testpgm/semtest1.d

exit 0
