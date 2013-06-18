#!/bin/bash

find . -name CMakeLists.txt -exec perl -i -pne 's/ SHARED/ STATIC/' '{}' ';'
rm -rf build
mkdir build
cd build
cmake .. -DUSE_SFST=OFF -DUSE_GV_WRITER=OFF -DUSE_LINK_GRAMMAR=OFF -DUSE_ASPELL=OFF -DUSE_PDF_READER=OFF
make -j2
