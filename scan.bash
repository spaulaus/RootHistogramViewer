#!/bin/bash
if [ -z "$1" ]
then 
    echo "We need to know which data set you want"
    echo "0 = Firmware 34455 - PR248 - iThemba LABS"
    echo "1 = Firmware 30474 - Pulser"
    echo "2 = Firmware 30474 - VANDLE ORNL 2016"
    exit
fi

if [[ $1 == 0 ]]
then
    data=${HOME}/data/ithemba/pr248/pr248-svp_003.ldf
    name=issue126-pr248
elif [[ $1 == 1 ]]
then
    data=${HOME}/data/utk/pixieworkshop/pulser_003.ldf
    name=issue126-pulser
else
    data=${HOME}/data/ornl/vandle2016/097rb_02.ldf
    name=issue126-ornl
fi 

prog=/home/vincent/programs/laughing-conqueror/cmake-build-release/Analysis/Utkscan/utkscan

$prog -b -i $data -c $name.xml -o $name
