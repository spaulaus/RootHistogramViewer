#!/bin/bash
prog=/home/vincent/projects/lc/jj-iThemba/cmake-build-release/Analysis/Utkscan/utkscan
data=${HOME}/data/ithemba/pr248/pr248-svp_003.ldf
name=jj-pr270

$prog -b -i $data -c $name.xml -o $name
