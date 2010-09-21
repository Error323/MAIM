#!/bin/sh
#
# Usage: $(basename $0) [VERSION]
#

set -u
DIR="$(dirname $0)/../"

# Update the VERSION file with the new version
echo "$1" > "$DIR/VERSION"

# Update data/AIInfo.lua with the new version
FILE="$DIR/data/AIInfo.lua"
LINE=`grep 'AI version - !This comment is used for parsing!' < $FILE`
NEWLINE="\t\tvalue  = '$1', -- AI version - !This comment is used for parsing!"

cp $FILE $FILE.bak
sed -r "s/$LINE/$NEWLINE/g" < $FILE.bak > $FILE
rm $FILE.bak
