#!/bin/bash

PROJECT_PATH=`pwd`
PROJECT_NAME=`basename $PROJECT_PATH`

mkdir -p bin
C:/msys64/mingw64/bin/clang++.exe -g *.cpp -lSplashKit -o ./bin/$PROJECT_NAME.exe