all: main.c app.c event.c render.c gstate.c input.c snk.c frt.c defs.c
	gcc -std=gnu99 -g main.c app.c event.c render.c input.c gstate.c snk.c frt.c defs.c -o snk -lncurses -lrt
	./snk
