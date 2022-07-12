PROGS = counter peterson mfence

all: $(PROGS)

counter: counter.c
	gcc -pthread -Wall counter.c -o counter
peterson: peterson.c
	gcc -pthread -Wall peterson.c -o peterson
mfence: mfence.c
	gcc -pthread -Wall mfence.c -o mfence

clean:
	rm -f $(PROGS)
