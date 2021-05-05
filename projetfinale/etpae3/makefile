car: main.o game.o fonction.o
	gcc main.o game.o fonction.o -o car -lSDL2_ttf -lSDL2_image -lSDL2 -lm
main: main.c game.h fonction.h variable.h
	gcc -c main.c
game.o : game.c fonction.h variable.h
	gcc -c game.c
fonction.o : fonction.c fonction.h variable.h
	gcc -c fonction.c
