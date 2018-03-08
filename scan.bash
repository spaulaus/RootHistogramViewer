#!/bin/bash

if [ -z "$1" ]
then
echo "We need a histogram name"
exit
fi

prog=${HOME}/programs/laughing-conqueror/cmake-build-release/Analysis/Utkscan/utkscan
data=${HOME}/data/ithemba/pr248/pr248-svp_003.ldf
cfg=issue126.xml

$prog -b -i $data -c $cfg -o $1
