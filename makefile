all: main.c app.c event.c render.c gstate.c input.c snk.c frt.c
	ctags-exuberant *.c *.h
	gcc -std=gnu99 -g main.c app.c event.c render.c input.c gstate.c snk.c frt.c -o snk -lncurses -lrt
	./snk
