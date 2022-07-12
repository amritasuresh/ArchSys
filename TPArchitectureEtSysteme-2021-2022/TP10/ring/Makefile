all: ring-pipe ring-net

ring-pipe: ring-pipe.c ring-pipe.h
	gcc -Wall ring-pipe.c -lm -o ring-pipe

ring-net: ring-net.c ring-net.h
	gcc -Wall ring-net.c -lm -o ring-net

clean:
	rm -f ring-pipe ring-net
