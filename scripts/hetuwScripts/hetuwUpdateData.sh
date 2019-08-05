#!/bin/sh

cd OneLifeData7
echo 
echo "UPDATE OneLifeData7"
echo
# git fetch https://github.com/jasonrohrer/OneLifeData7
git fetch --tags https://github.com/jasonrohrer/OneLifeData7
latestTaggedVersionB=`git for-each-ref --sort=-creatordate --format '%(refname:short)' --count=1 refs/tags/OneLife_v* | sed -e 's/OneLife_v//'`
git checkout -q OneLife_v$latestTaggedVersionB
echo "checkout data version $latestTaggedVersionB"
cd ..

./hetuwClearCache.sh
