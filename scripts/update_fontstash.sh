#! /bin/sh

SRC=fontstash
DEST=../libs/fontstash

###

cd $(dirname $0)

# get latest source
git clone https://github.com/memononen/fontstash.git

# create dirs
mkdir -p $DEST/include

# copy license
cp -v $SRC/LICENSE.txt $DEST

# copy sources
cp -v $SRC/src/* $DEST/include

# also grab the latest std_truetype version
curl -O https://raw.githubusercontent.com/nothings/stb/master/stb_truetype.h
mv -v stb_truetype.h $DEST/include 

# cleanup
rm -rf $SRC

