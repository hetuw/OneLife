#!/bin/sh

echo
echo "pulling update from jason to linux"
echo

cd OneLifeData7
echo 
echo "UPDATE OneLifeData7"
echo
git pull https://github.com/jasonrohrer/OneLifeData7
cd ..
cd linux
cd OneLife
echo 
echo "UPDATE OneLife"
echo
git pull https://github.com/jasonrohrer/OneLife
cd ..
cd minorGems
echo 
echo "UPDATE minorGems"
echo
git pull https://github.com/jasonrohrer/minorGems
cd ..
cd ..
