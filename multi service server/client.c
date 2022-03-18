#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

/*** globals ***/
char msg[500];

void *recvmg(void *my_sock)
{
	int sock = *((int *)my_sock);
	int len;
	// client thread always ready to receive message
	while((len = recv(sock,msg,500,0)) > 0) {
	        //char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 23\n\n";
                msg[len] = '\0';
                //strcat(hello,msg);
                //write(sock,hello,strlen(hello));
		fputs(msg,stdout);
		
	}
}

int main(int argc,char *argv[]){
	pthread_t recvt;
	int len;
	int sock;
	char send_msg[500];
	struct sockaddr_in ServerIp;
	// since we have multiple clients
	char client_name[100];
	strcpy(client_name, argv[1]);
	
	// socket creation
	sock = socket( AF_INET, SOCK_STREAM,0);
	// port to connect to clients
	ServerIp.sin_port = htons(1234);
	// address family used when setting up the socket 
	ServerIp.sin_family= AF_INET;
	ServerIp.sin_addr.s_addr = inet_addr("127.0.0.1");
	/*** socket connection ***/
	if( (connect( sock ,(struct sockaddr *)&ServerIp,sizeof(ServerIp))) == -1 )
		printf("n connection to socket failed n");
	
	// client thread creation to receive for a message
	pthread_create(&recvt,NULL,(void *)recvmg,&sock);
	
	//ready to read a message from console
	while(fgets(msg,500,stdin) > 0) {
	        // copying clients name
		strcpy(send_msg,client_name);
		strcat(send_msg,":");
		// concatenating the  msg to be printed
		strcat(send_msg,msg);
		len = write(sock,send_msg,strlen(send_msg));
		if(len < 0) 
			printf("n message not sent n");
	}
	
	//thread is closed
	pthread_join(recvt,NULL);
	close(sock);
	return 0;
}
