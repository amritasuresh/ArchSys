void free_packet (void *buffer); 
void send_packet (int to_socket, void *packet, unsigned short int length);
void wait_for_packet (int from_socket); 
unsigned short int receive_packet (int from_socket, void **buffer);
