#!/bin/bash

if ! g++ -o main.out main.cpp; then
    exit 1
fi

./main.out
echo ""
