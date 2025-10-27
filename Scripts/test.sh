#!/bin/bash

PROJECT_PATH=`pwd`
PROJECT_NAME=`basename $PROJECT_PATH`

CURRENT_FILE_PATH=$1
CURRENT_FILE_FOLDER=`dirname $CURRENT_FILE_PATH`

C:/msys64/mingw64/bin/g++.exe -g $(find $CURRENT_FILE_FOLDER -type f -name '*.cpp' -print) -Wall -Wextra -lSplashKit -L"$PROJECT_PATH/lib" -I"$PROJECT_PATH/include" -lcatch_amalgamated -o "${CURRENT_FILE_PATH%.cpp}.exe"