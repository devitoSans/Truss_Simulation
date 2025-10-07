#!/bin/bash

PROJECT_PATH=`pwd`
PROJECT_NAME=`basename $PROJECT_PATH`

mkdir -p bin
C:/msys64/mingw64/bin/g++.exe -g $(find . -type f -name '*.cpp' ! -name 'tester.cpp' -print) -lSplashKit -o ./bin/$PROJECT_NAME.exe
find . -type f -name 'tester.exe' -exec rm -f {} +
find . -type f -name 'myeasylog.log' -exec rm -f {} +