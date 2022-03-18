#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdlib.h>

/*** globals ***/
char msg[500];

void *recvmg(void *my_sock)
{
	int sock = *((int *)my_sock);
	int len;
	// client thread always ready to receive message
	while((len = recv(sock,msg,500,0)) > 0) {
	        
                msg[len] = '\0';
                printf("response from server -\t");
		fputs(msg,stdout);
		
	}
}

void POST(int clientfd,char msg[]) {
  
    char data[100];
    strcpy(data,msg);
    printf("message : %s",data);
    char XmlRequest[250]= {0};
    char ServiceMethod[]= "multiserver.c";
    char request[150]= {0};
    char hostIp[30]="localhost";
    char port[] = "8080";
    
    sprintf(request,"http://%s:%s/%s HTTP/1.1",hostIp,port,ServiceMethod);
    //printf("Method and Resource path is below:\n\n\n");
    //printf("%s",request);
    
    strcat(hostIp,":");
    strcat(hostIp,port);
    
    //printf("\n\nHOST header is below:\n\n\n");
    //printf("%s",hostIp);
    
    snprintf(XmlRequest,(size_t)"POST %s\r\nHost: %s\r\nContent-Type: text/plain\r\nContent-Length: %d\r\n\r\n%s\r\n",request,hostIp,strlen(data),data);
    printf("\nPOST Request sent to the server:");
    //printf("%s\n",aszXmlRequest);
    
    char req[1000] = {0};
    
    sprintf(req,"POST %s\r\n",XmlRequest);

    strcat(req,data);
    printf("%s\t",req);
    send(clientfd,req,strlen(req),0); 
}


int main(int argc,char *argv[]){

        if (argc != 2) {
           fprintf(stderr, "USAGE: ./a.out <client name>\n");
           return 1;
        }  

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
		printf("\nsocket connection failed\n");
	
	// client thread creation to receive for a message
	pthread_create(&recvt,NULL,(void *)recvmg,&sock);
	
	//ready to read a message from console
	while(fgets(msg,500,stdin) > 0) {
	        if ((strncmp(msg, "exit", 4)) == 0) {
			printf("client Exit...\n");
			return -1;
		}
	        // copying clients name
		strcpy(send_msg,client_name);
		strcat(send_msg,":");
		// concatenating the  msg to be printed
		strcat(send_msg,msg);
		len = write(sock,send_msg,strlen(send_msg));
		POST(sock,send_msg);
		if(len < 0) 
			printf("\nfailed to send\n");
	}
	
	//thread is closed
	pthread_join(recvt,NULL);
	close(sock);
	return 0;
}
