#!/bin/sh
#
# Usage: $(basename $0) [VERSION] [ABSOLUTE PATH]
#

set -u
echo "$1" > "$2/VERSION"

FILE="$2/data/AIInfo.lua"
LINE=`grep 'AI version - !This comment is used for parsing!' < $FILE`
NEWLINE="\t\tvalue  = '$1', -- AI version - !This comment is used for parsing!"

cp $FILE $FILE.bak
sed -r "s/$LINE/$NEWLINE/g" < $FILE.bak > $FILE
rm $FILE.bak
