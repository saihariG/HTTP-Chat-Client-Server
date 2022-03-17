#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h> 

#define MAX 100
#define PORT 8080
#define SA struct sockaddr

int main()
{
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;

        char buffer[MAX];
	int n;
	  
	// socket creation
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else {
		printf("Socket successfully created..\n");
	}
	
	bzero(&servaddr, sizeof(servaddr));

	// assigning IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP address
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	}
	else {
		printf("Socket successfully binded..\n");
	}

	// Now server is ready to listen 
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	else {
		printf("Server listening..\n");
	}
	len = sizeof(cli);

	// Accept the data packet from client
	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		printf("server accept failed...\n");
		exit(0);
	}
	else {
		printf("server accept the client...\n");
        }
        
	// infinite loop for chatting between client and server 
	while(1) {
		bzero(buffer, MAX);

		// read the message from client and copy it in buffer
		read(connfd, buffer, sizeof(buffer));
		// print buffer which contains the client contents
		printf("From client: %s\t To client : ", buffer);
		bzero(buffer, MAX);
		n = 0;
		// copy server message in the buffer
		while ((buffer[n++] = getchar()) != '\n')
			;

		// send that buffer to client
		write(connfd, buffer, sizeof(buffer));

		// if msg contains "Exit" then server exit and chat ended.
		if (strncmp("exit", buffer,4) == 0) {
			printf("Server Exit...\n");
			break;
		}
	}

	// After chatting close the socket
	close(sockfd);
}

