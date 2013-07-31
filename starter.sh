#!/bin/bash

name=$1
jobs=$2
echo $name

mkdir -p $name

sed -e "s,ARGUMENTS,$*,g" -e "s/NAME/$name/g" sub.sub > $name/sub.sub
for i in `seq $jobs`
do
	echo Starting $i
	qsub $name/sub.sub
done
