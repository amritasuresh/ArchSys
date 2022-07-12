.c.o:
	gcc -Wall -g $< -c -o $*.o

CLIENT-OBJ = client.o packets.o network.o
SERVER-OBJ = server.o packets.o network.o

all: client server
 
client: $(CLIENT-OBJ) 
	gcc $(CLIENT-OBJ) -o client 
 
server: $(SERVER-OBJ) 
	gcc $(SERVER-OBJ) -o server 

clean:
	\rm -f client server $(CLIENT-OBJ) $(SERVER-OBJ) core

