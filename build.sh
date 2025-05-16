#!/bin/bash

if [ ! -d "build" ]; then
    mkdir build
fi

command="gcc -std=c23 -g ../source/main.c -o web-server"

cd build

bear -- $command
$command

if [ "$1" == "run" ]; then
    ./web-server
fi

cd ..
