#!/bin/bash

gcc -Wall nproc.c -o nproc

diff <(./nproc) <(nproc)

rm -f ./nproc
