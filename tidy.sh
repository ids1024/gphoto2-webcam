#!/bin/sh

CFLAGS="--std=c++17
       -isystem /usr/include/qt
       -isystem /usr/include/qt/QtWidgets"

clang-tidy -quiet src/*.cpp $@ -- $CFLAGS
