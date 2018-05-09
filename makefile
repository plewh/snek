all: app.c defs.c gstate.c main.c snek.c
	clear
	gcc -std=gnu99 app.c defs.c gstate.c main.c snek.c -o snk -lncurses
	./snk
