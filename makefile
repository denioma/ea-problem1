cc = g++
cflags = -Wall -Wextra -std=c++17 -O2 -lm
src = main.cpp
out = main

all: network

network: $(src)
	$(cc) $(cflags) $< -o $(out)