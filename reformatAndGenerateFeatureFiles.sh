#!/bin/bash

if [ "$1" == "" -o "$2" == "" -o "$3" == "" ]
then
    echo "Usage: reformatAndGenerateFeatureFiles.sh inputDirectory"
    echo "                                          gameOutputDirectory"
    echo "                                          featureOutputDirectory"

    exit 1
fi

if [ ! -d $3 ]
then
    mkdir $3
fi

echo Generating games and features for the *.sgf files in $1,
echo placing the outputing *.sgfo games in $2 and *.sgfof
echo feature files in $3...
echo
echo -n Step 1: Outputting simplified game records...

./reformatGames.sh $1 $2

echo Step 1: Complete
echo 
echo -n Step 2: Calculating block features for each game...

./generateFeatureFiles $2 $3

echo Step 2: Complete
echo
echo Done.
