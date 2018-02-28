#!/bin/bash

if [ -z "$1" ]
then
echo "We need a histogram name"
exit
fi

data=${HOME}/data/ithemba/pr248/pr248-svp_003.ldf
cfg=issue120.xml

./utkscan -b -i $data -c $cfg -o $1
