#!/bin/bash

name=$1
jobs=$2
echo $name

mkdir -p $name

sed -e "s,ARGUMENTS,$*,g" -e "s/NAME/$name/g" sub.sub > $name/sub.sub
qsub $name/sub.sub
