#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

/*** globals ***/

// for multithreading
pthread_mutex_t mutex;
// maximum number of clients that can be handled
int clients[20];
int n=0;

void sendtoall(char *msg,int sock){
	int i;
	// thread locking using mutex to ensure synchronization of shared resources    
	pthread_mutex_lock(&mutex);
	for(i = 0; i < n; i++) {
		if(clients[i] != sock) {
		       // sending messages to all clients
			if(send(clients[i],msg,strlen(msg),0) < 0) {
				printf("failed to send\n");
				continue;
			}else {
			       printf("\nresponse sent from server\n\n");
			}
		}
	}
	
	// unlocking the thread once the job is finished
	pthread_mutex_unlock(&mutex);
}

// receiving data from client socket
void *recvmg(void *client_sock){
	int sock = *((int *)client_sock);
	char msg[500];
	int len;
	while((len = recv(sock,msg,500,0)) > 0) {
	        printf("request received from client");
		msg[len] = '\0';
		sendtoall(msg,sock);
	}
	
}

int main(){
	struct sockaddr_in ServerIp;
	pthread_t recvt;
	// initializing socket 
	int sock=0 , Client_sock=0;
	
	// address family used when setting up the socket 
	ServerIp.sin_family = AF_INET;
	// pick a specific number. clients will need to know a port number to connect to eg : 8080
	ServerIp.sin_port = htons(1234);
	// special address eg : 0.0.0.0
	ServerIp.sin_addr.s_addr = inet_addr("127.0.0.1");
	// socket creation
	sock = socket( AF_INET , SOCK_STREAM, 0 );
	
	int on = 1;
        setsockopt (sock , SOL_SOCKET, SO_REUSEADDR, &on, sizeof (int)); 

	if( bind( sock, (struct sockaddr *)&ServerIp, sizeof(ServerIp)) == -1 )
		printf("cannot bind, error!! \n");
	else
		printf("Server Started\n");
		
	
        // listen system call tells a socket that it should accepting incoming connections
        // 2nd param : maximum number of pending connections 	
	if( listen( sock ,20 ) == -1 )
		printf("listening failed \n");
		
	// infinite loop for broadcasting messages	
	while(1){
	        // accepting incoming connections from client
		if( (Client_sock = accept(sock, (struct sockaddr *)NULL,NULL)) < 0 )
			printf("accept failed\n");
		pthread_mutex_lock(&mutex);
		clients[n]= Client_sock;
		n++;
		// creating a thread for each client 
		pthread_create(&recvt,NULL,(void *)recvmg,&Client_sock);
		pthread_mutex_unlock(&mutex);
	}
	return 0; 
	
}
