#!/bin/bash

if [ "$1" == "" -o "$2" == "" -o "$3" == "" ]
then
    echo "Usage: reformatAndGenerateBoard.sh inputDirectory"
    echo "                                   gameOutputDirectory"
    echo "                                   boardOutputDirectory"

    exit 1
fi

echo -n Generating games and features for the *.sgf files in $1,
echo -n placing the outputing *.sgfo games in $2 and *.sgfob
echo board files in $3...
echo
echo -n Step 1: Outputting simplified game records...

if [ ! -d $2 ]
then
    mkdir $2
fi

./reformatGames.sh $1 $2

echo Step 1: Complete
echo 
echo -n Step 2: Generating the final boards for each game...

if [ ! -d $3 ]
then
    mkdir $3
fi

./gobot -generateBoards $2 $3

echo Step 2: Complete
echo
echo Done.
