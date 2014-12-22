#!/bin/bash

if [ "$1" == "" -o "$2" == "" ]
then
    echo Usage: reformatGames inputDirectory outputDirectory
    exit 1
fi

if [ ! -d $2 ]
then
    mkdir $2
fi

FILES=$(find $1 -type f -name '*.sgf')

I=0

for FILE in $FILES
do
    ./reformat $FILE $2/$(basename $FILE)o &> /dev/null

    if [ $((I % 100)) == 0 ]
    then
        echo -n .
    fi

    I=$((I + 1))
done

echo

