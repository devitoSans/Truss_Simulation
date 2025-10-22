#!/bin/bash

PROJECT_PATH=`pwd`
PROJECT_NAME=`basename $PROJECT_PATH`

find ${PROJECT_PATH}/sources -type f | xargs wc -l