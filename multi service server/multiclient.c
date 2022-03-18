#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthreaed.h>
#include <errno.h>

/*** globals ***/
char msg[500];

void *recvmg(void *client_socket) {
     int sock = *((int *)client_socket);
     int len;
     
     // client thread waiting to receive message
     while((len = recv(sock,msg,500,0)) > 0) {
         msg[len] = '';
         fputs(msg,stdout);
     }
}

int main(int argc,char *argv[]) {
   
    pthread_t recvt;
    
    int len, sock;
    struct sockaddr_in server_ip;
    
    if(argc != 2) {
       fprintf(stderr,"USAGE: ./client <client name>"); 
       return 1;
    }	      

    char sent_msg[500];
    // since we have multiple clients
    char client_name[100];
    strcpy(client_name,argv[1]);   

    // socket creation
    sock = socket(AF_INET,SOCK_STREAM,0);
  
    // address family used when setting up the socket 
    server_ip.sin_family = AF_INET;
    
    // port to connect to clients
    server_ip.sin_port = htons(1234);

    // special ip address
    server_ip.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    /*** socket connection ***/
    if((connect(sock,(struct sockaddr *)&server_ip,sizeof(server_ip))) == -1) {
        printf("\nsocket connection failed");
    }
    
    // client thread creation to receive message
    pthread_create(&recvt,NULL,(void *)recvmg,&sock);     

    // reading a message from console
    while(fgets(msg,500,stdin) > 0) { 
         // copying client's name
         strcpy(sent_msg,client_name);
         strcat(sent_msg,":");
         // concatenating the  msg to be printed
         strcat(sent_msg,msg);  
         len = write(sock,sent_msg,strlen(sent_msg));
         if(len < 0) {
             printf("\nfailed to send\n");
         }      
    } 
    
    // closing the socket
    pthread_join(recvt,NULL);   
    close(sock);
    return 0;
}






