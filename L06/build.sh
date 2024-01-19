#!/bin/bash
gcc -g $1.c -o $1 -Wall && ./$1
