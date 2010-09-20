#!/bin/sh
#
# Usage: $(basename $0) [VERSION]
#

set -u
echo "$1" > "$2/VERSION"
