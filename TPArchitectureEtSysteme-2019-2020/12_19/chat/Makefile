CC = gcc
CFLAGS = -pthread
SRCS = client.c packets.c

.SUFFIXES: .out .c

client.out: client.c packets.c packets.h
	${CC} ${CFLAGS} -o ${.TARGET} packets.c client.c
