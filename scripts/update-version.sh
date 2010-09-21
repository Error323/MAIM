#!/bin/sh
#
# Usage: $(basename $0) [VERSION]
#

set -u
echo "$1" > "$2/VERSION"

FILE="$2/data/AIInfo.lua"
cp $FILE $FILE.bak
sed -r "s/'[[:digit:]]\.[[:digit:]]\.[[:digit:]]'/'$1'/g" < $FILE.bak > $FILE
cp $FILE $FILE.bak
sed -r "s/'[[:alnum:]]*\-[[:digit:]]*\-[[:alnum:]]*'/'$1'/g" < $FILE.bak > $FILE
rm $FILE.bak
