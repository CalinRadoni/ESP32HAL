#!/bin/bash

declare -i res

. ${IDF_PATH}/export.sh

cd /github/workspace/ESP32HAL/test

idf.py build
res=$?

echo "::set-output name=result::$res"
