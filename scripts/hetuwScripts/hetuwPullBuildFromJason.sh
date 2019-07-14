#!/bin/sh

sh ./hetuwPullFromJason.sh

echo
echo "next step compiling"
echo "press enter to continue"
read ANSWER

cd linux
./runToBuild 1
cd ..

echo
echo "next step pushing to hetuw"
echo "press enter to continue"
read ANSWER

sh ./hetuwPushToHetuw.sh

echo
echo "next step update windows"
echo "press enter to continue"
read ANSWER

sh ./hetuwPullLatest.sh windows 3

echo
echo "next step compiling all and clearing cache"
echo "press enter to continue"
read ANSWER

sh ./hetuwCompileAll.sh

sh ./hetuwClearCache.sh

