#!/usr/bin/env bash
make clean
make
# part 1
#gdb -ex 'break main' -ex 'tui enable' -ex 'run' ./csim
#part 2
./test-trans -M 64 -N 64
