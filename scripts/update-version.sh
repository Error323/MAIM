#!/bin/sh
#
# Usage: $(basename $0) [VERSION]
#

set -u
echo "$1" > "$2/VERSION"

FILE="$2/data/AIInfo.lua"
cp $FILE $FILE.bak
sed -e "s/[0-9]\.[0-9]\.[0-9]/$1/g" < $FILE.bak > $FILE
rm $FILE.bak
