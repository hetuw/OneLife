#!/bin/sh

cd linux
cd OneLife
echo 
echo "PUSH OneLife"
echo
git push https://github.com/selb/YumLife
cd ..
cd minorGems
echo 
echo "PUSH minorGems"
echo
git push https://github.com/selb/YumLifeMinorGems
cd ..
cd ..
