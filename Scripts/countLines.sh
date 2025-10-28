#!/bin/bash

PROJECT_PATH=`pwd`
PROJECT_NAME=`basename $PROJECT_PATH`

find ${PROJECT_PATH}/sources \( -name '*.cpp' -o -name '*.hpp' \) ! -name "tester.cpp" -type f | xargs wc -l
# find ${PROJECT_PATH}/sources \( -name '*.cpp' -o -name '*.hpp' \) -type f | xargs wc -l