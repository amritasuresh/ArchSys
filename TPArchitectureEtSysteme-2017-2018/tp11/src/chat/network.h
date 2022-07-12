// can be passed to select as first argument
#define MAXFILES 1024

// print message and exit
void fatal_error (char *msg);

// Create and return a TCP socket.
int create_tcp_socket ();

// This connects the given socket to the server at the given port.
void connect_socket_to_server (int sock, char *hostname, int port);

// connect the given socket to the given port
void bind_to_port (int sock, int port);

// Turn socket into listen mode
void listen_at_socket (int sock);

// Wait for a client connection at socket and return the socket
// for communicating with that client.
int accept_client (int sock);

// Send a string to the given socket.
void send_string (int to_socket, char *str);

// Read a string from the given socket. Memory for the string
// is automatically allocated. It is your responsibility to
// free that memory when it is no longer needed.
char* receive_string (int from_socket);
