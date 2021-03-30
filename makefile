sdl_options = -lSDL2
options 	= -Wall -Wextra -pedantic -O3

main : bin/sdl.o bin/main.o
	g++ bin/sdl.o bin/main.o $(sdl_options) -o main

bin/sdl.o : sdl.cpp sdl.h
	g++ -c sdl.cpp $(options) -o bin/sdl.o

bin/main.o : main.cpp sdl.h
	g++ -c main.cpp -mavx $(options) -o bin/main.o
