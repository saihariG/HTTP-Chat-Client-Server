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
        //int count = 1;
	int sock = *((int *)my_sock);
	int len;
	// client thread always ready to receive message
	while((len = recv(sock,msg,500,0)) > 0) {
	        
                msg[len] = '\0';
                printf("response from server -\t");
	 	fputs(msg,stdout);
	 	  	
	}
}

void POST(int clientfd,char *msg) {
  
    char data[100]; 
    strcpy(data,msg);
    //printf("passed msg : %s",data);
    char XmlRequest[250]= {0};
    char ServiceMethod[]= "multiserver.c";
    char request[150]= {0};
    char hostIp[30]="localhost";
    char port[] = "8080";
    
    sprintf(request,"http://%s:%s/%s HTTP/1.1",hostIp,port,ServiceMethod);
       
    strcat(hostIp,":");
    strcat(hostIp,port);
    

    snprintf(XmlRequest,(size_t)"POST %s\r\nHost: %s\r\nContent-Type: text/plain\r\nContent-Length: %d\r\n\r\n%s\r\n",request,hostIp,strlen(data),data);
           
    char req[1000] = {0};
    
    sprintf(req,"POST %s ",XmlRequest);

    strcat(req,data);
    //printf("post req to send is : %s",req);
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
		strcat(send_msg,":"); // client1:
		// concatenating the  msg to be printed
		strcat(send_msg,msg); // client1 : hi
		len = write(sock,send_msg,strlen(send_msg));
		//printf("message to be posted : %s",msg);
		POST(sock,msg); 
		if(len < 0) {
			printf("\nfailed to send\n");
		}
	}
	

	pthread_join(recvt,NULL);
	close(sock);
	return 0;
}
