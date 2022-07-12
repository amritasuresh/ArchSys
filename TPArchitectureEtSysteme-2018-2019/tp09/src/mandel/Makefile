PROGS = mandel

all: $(PROGS)

mandel: mandel.c mandel.h
	gcc -pthread -Wall mandel.c -lSDL -lX11 -lm -o mandel

clean:
	rm -f $(PROGS)
