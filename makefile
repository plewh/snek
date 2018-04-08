all: main.c
	clear
	gcc -std=gnu99 main.c -o snk -lncurses
	./snk