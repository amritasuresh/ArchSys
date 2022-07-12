PROGS = counter order procon mandel

all: $(PROGS)

counter: counter.c
	gcc -pthread -Wall counter.c -o counter

order: order.c
	gcc -pthread -Wall order.c -o order

procon: procon.c
	gcc -pthread -Wall procon.c -o procon

mandel: mandel.c mandel.h
	gcc -pthread -Wall mandel.c -lSDL -lX11 -lm -o mandel

clean:
	rm -f $(PROGS)
