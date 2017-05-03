#!/bin/bash

clear

g++ -c -std=c++11 src/*.cpp
g++ -o starship *.o -lSDL2 -lSDL2_image
./starship
