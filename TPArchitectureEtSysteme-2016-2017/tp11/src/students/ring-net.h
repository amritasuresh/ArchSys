void protocole ();

// délai aléatoire, distribution exponentielle
void delay ()
{
	double d = drand48();
	double r = -log(d) * DELAY;
	usleep(r*1e6);
}

// Recevoir un message (c est parmi 'n','x','w', et d un identifiant)
void recevoir (char *c, int *d)
{
	char pb;	// pour EOL
	fscanf(node.chan_in,"%c %d%c",c,d,&pb);
}

// Envoyer un message
void envoyer (char c, int d)
{
	delay(); // pour rendre le comportement un peu moins déterministe
	fprintf(node.chan_out,"%c %d\n",c,d);
}

// Émettre un message sur l'écran. Rajoute une nouvelle ligne à la fin.
void message (char* msg, ...)
{
        va_list args;
	printf("[nœud %d] ",node.id);
        va_start(args,msg);
        vprintf(msg,args);
        va_end(args);
	puts("");
}

// créer les structures autour d'un nœud
void create_node ()
{
	srand48(time(NULL) + getpid());
	node.id = lrand48() & 0xffff;

	// create streams from file descriptors
	node.chan_in = fdopen(node.fd_in,"r"); 
	node.chan_out = fdopen(node.fd_out,"w"); 

	// make both files line-buffered (saves fflush calls)
	setlinebuf(node.chan_in); setlinebuf(node.chan_out);
}

// Accepter une connection sur port, renvoie un descripteur.
int accept_connection (int port)
{
        struct sockaddr_in server_address;

        // create a TCP socket
        int s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

        // connect the socket to the chosen port
        server_address.sin_family = AF_INET;
        server_address.sin_addr.s_addr = INADDR_ANY;
        server_address.sin_port = htons(port);
        if (bind(s,(struct sockaddr *)&server_address,sizeof server_address))
	{
                fprintf(stderr,"Error: can't bind to port %d, "
				"still occupied?",port);
		exit(1);
	}

        // make the socket listen for connections
        listen(s,1);

	printf("Waiting for connection on port %d ...\n",port);
        while (1)
        {
                // let a client connect to the socket
                int client_socket = accept(s,NULL,NULL);

                if (client_socket < 0)
                {
                        if (errno == EINTR || errno == EWOULDBLOCK) continue;
			fprintf(stderr,"Error: client connection failed\n");
			exit(1);
                }

		printf("Connection accepted!\n");
		return client_socket;
        }
}

// Connecter au serveur sur hostname:port, renvoie un descripteur
int connect_to_server (char *hostname, int port)
{
        // Create a TCP socket
        int s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

        // Find the IP address of the host machine (where the server is
        // supposed to run).
        struct hostent *host_address = gethostbyname(hostname);
        if (!host_address)
	{
		fprintf(stderr,"Error: unknown host %s\n",hostname);
		exit(1);
	}

        // Build the address from hostname and port.
        struct sockaddr_in server_address;
        server_address.sin_family = AF_INET;
        memcpy(&server_address.sin_addr,
                host_address->h_addr_list[0],
                host_address->h_length);
        server_address.sin_port = htons(port);

        // Connect to the server
	printf("Connecting to server at %s:%d ...\n",hostname,port);
	while (1)
	{
		if (connect(s,(struct sockaddr *)&server_address,
                     sizeof server_address) == 0)
		{
			printf("Connected!\n");
			return s;
		}
		sleep(3);
		printf("retrying...\n");
	}

}

