#!/bin/bash

find . -name CMakeLists.txt -exec perl -i -pne 's/ SHARED/ STATIC/' '{}' ';'
