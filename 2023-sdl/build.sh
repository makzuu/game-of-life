#!/usr/bin/sh

set -xe

cc -o a.out main.c -Wall -Wextra `sdl2-config --cflags --libs`
