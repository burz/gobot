#!/bin/bash

N=$(($1 + $2))

for i in ${3}/*; do
  [ $((N--)) = 0 ] && break
  [ $((N)) -lt $(($2)) ] && cp "$i" "$4"
done
