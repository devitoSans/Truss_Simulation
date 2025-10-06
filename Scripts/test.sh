#!/bin/bash

PROJECT_PATH=`pwd`
PROJECT_NAME=`basename $PROJECT_PATH`

CURRENT_FILE_PATH=$1
C:/msys64/mingw64/bin/clang++.exe -g $CURRENT_FILE_PATH -lSplashKit -L"$PROJECT_PATH/lib" -I"$PROJECT_PATH/include" -lcatch_amalgamated -o "${CURRENT_FILE_PATH%.cpp}.exe"
${CURRENT_FILE_PATH%.cpp}.exe