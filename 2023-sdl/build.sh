#!/usr/bin/sh

set -xe

cc -o gol main.c -Wall -Wextra `sdl2-config --cflags --libs`
