#!/bin/bash

name=$1
echo $name

mkdir -p $name

cat sub.sub | sed -e "s,ARGUMENTS,$*,g" -e "s/NAME/$name/g"> $name/sub.sub
