#! /bin/bash

WD=$(dirname $0)

SRC=fontstash
DEST=../libs/fontstash

###

cd $WD

# get latest source
git clone https://github.com/memononen/fontstash.git

# remove lua standalone console and compiler sources
#rm -v $SRC/src/lua.c $SRC/src/luac.c

# create dirs
mkdir -p $DEST/include

# copy license
cp -v $SRC/LICENSE.txt $DEST

# copy sources
cp -v $SRC/src/* $DEST/include

# cleanup
rm -rfv $SRC

