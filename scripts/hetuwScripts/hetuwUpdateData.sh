#!/bin/sh

cd OneLifeData7
echo 
echo "UPDATE OneLifeData7"
echo
git pull https://github.com/jasonrohrer/OneLifeData7

echo 
echo "CLEAR CACHE OneLifeData7"
echo
rm */cache.fcz
