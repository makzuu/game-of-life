#!/usr/bin/sh

set -xe

cc -g -o a.out main.c -Wall -Wextra `sdl2-config --cflags --libs`
