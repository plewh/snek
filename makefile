all: main.c snek.c app.c defs.c
	clear
	gcc -std=gnu99 main.c snek.c app.c defs.c -o snk -lncurses
	./snk
