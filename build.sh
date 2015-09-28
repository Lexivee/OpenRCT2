#!/bin/bash

set -e

cachedir=.cache
mkdir -p $cachedir

if [[ ! -d build ]]; then
	mkdir -p build
fi

libversion=2
libVFile="./libversion"
libdir="./lib"
currentversion=0
needsdownload="true"

if [ -f $libVFile ]; then 
    while read line; do    
        currentversion=$line  
        continue 
    done < $libVFile
fi

if [ $currentversion -ge $libversion ]; then
    needsdownload="false"
fi

if [ ! -d $libdir ]; then
    needsdownload="true"    
fi
    
if [[ "$needsdownload" = "true" ]]; then
    echo "New libraries need to be downloaded, the script might ask you for sudo access password"
    rm -rf ./lib
    if [[ -f $cachedir/orctlibs.zip ]]; then 
        rm -rf $cachedir/orctlibs.zip
    fi
    if [[ -d /usr/local/cross-tools/orctlibs ]]; then
        sudo rm -rf /usr/local/cross-tools/orctlibs
    fi
     if [[ -d $cachedir/orctlibs ]]; then
        rm -rf $cachedir/orctlibs
    fi
    curl https://dl.dropboxusercontent.com/u/1323345/orctlibs.zip -o $cachedir/orctlibs.zip
    mkdir -p $cachedir/orctlibs
    pushd $cachedir/orctlibs
        unzip -uaq ../orctlibs.zip 
    popd
    sudo mkdir -p /usr/local/cross-tools/orctlibs
    mkdir -p lib
    sudo cp -rf $cachedir/orctlibs/glob/* /usr/local/cross-tools/orctlibs/.
    cp -rf $cachedir/orctlibs/local/* ./lib/. 
    echo $libversion > $libVFile
fi

pushd build
	echo OPENRCT2_CMAKE_OPTS = $OPENRCT2_CMAKE_OPTS
	cmake -DCMAKE_BUILD_TYPE=Debug $OPENRCT2_CMAKE_OPTS ..
	make
popd

if [[ ! -h openrct2.dll ]]; then 
    ln -s build/openrct2.dll openrct2.dll
fi

if [[ -z "$DISABLE_G2_BUILD" ]]; then
    echo Building: data/g2.dat
    ./build_g2.sh > /dev/null 2>&1
fi

if [[ -t 1 ]]; then
    echo -e "\nDone! Run OpenRCT2 by typing:\n\n\033[95mwine openrct2.exe\n\033[0m"
else
    echo -e "\nDone! Run OpenRCT2 by typing:\n\nwine openrct2.exe\n"
fi
 
