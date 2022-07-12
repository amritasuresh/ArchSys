#include <stdio.h>
#include <errno.h>  
#include <stdlib.h>  
#include <ulimit.h>
#include <unistd.h>
#include <sys/socket.h>  
#include <sys/time.h>  
#include <sys/types.h>  
#include <netinet/in.h>
#include <string.h>
  
#include "packets.h" 
#define MAXFILES 20

void fatal_error(char *err_msg)
{
        fprintf(stderr,"%s\n",err_msg);
        exit(0);
}

void free_buffer (void *buffer)   
{   
	free(buffer);   
}   
 
void send_buffer (int to_socket, void *buffer, int length) 
{ 
	int result, sent; 
   
	for (sent = 0; sent < length; )
	{ 
		result = write(to_socket, buffer + sent, length - sent); 
		if (result < 0 && errno != EINTR)   
			fatal_error("Can't send packet");   
		if (!result) break;
		sent += result;
	}
   
	if (sent != length) 
		fatal_error("Can't send complete packet");   
} 
 
void send_packet (int to_socket, void *packet, unsigned short int length)
{
	short int l = htons(length);
	send_buffer(to_socket,&l,sizeof(short int)); 
	send_buffer(to_socket,packet,length); 
}
 
void send_string (int to_socket, char *str)
{
	send_packet(to_socket,str,strlen(str)+1);
}

void wait_for_packet(int from_socket)   
{   
	fd_set readfds;   
	int result;   
   
	for (;;)
	{   
		FD_ZERO(&readfds);   
		FD_SET(from_socket, &readfds);   
   
		result = select(MAXFILES, &readfds, NULL, NULL, NULL);   

		if (result < 0 && errno != EINTR)   
			fatal_error("Can't wait for msg");   
   
		if (FD_ISSET(from_socket,&readfds))   
			break;   
	}
}   

void receive_buffer (int from_socket, void *buffer, int length)
{ 
	int result,rcvd; 
 
	for (rcvd = 0; rcvd < length; )
	{ 
		wait_for_packet(from_socket);   
		result = read(from_socket, buffer + rcvd, length - rcvd); 
		if (result < 0 && errno != EINTR)   
			fatal_error("Can't receive packet");   
		if (!result) break;   
   
		rcvd += result;   
	}   
   
	if (rcvd != length) 
		fatal_error("Got incomplete packet");   
} 
 
unsigned short int receive_packet (int from_socket, void **buffer)
{   
	unsigned short int length;
	receive_buffer(from_socket, &length, sizeof(short int)); 
	length = ntohs(length);
	*buffer = malloc(length);
	receive_buffer(from_socket, *buffer, length); 
	return length;
}   
