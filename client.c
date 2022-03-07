#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<unistd.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>

// default port address for connection
#define PORT 8080

int main(int argc,char const *argv[]) {
    int sock = 0;
    long valread;
    // defined in the header netinet/in.h
    struct sockaddr_in serv_addr;
    char *hello = "hello from Client";
    char buffer[1024] = {0};
    
    // creating a socket
    if((sock = socket(AF_INET,SOCK_STREAM,0)) < 0) {
    	printf("\n Socket creation error\n");
    	return -1;
    }

    // fill a block of memory     
    memset(&serv_addr,'0',sizeof(serv_addr));
    // address family used to set up the socket
    serv_addr.sin_family = AF_INET;
    // port to connect with the server
    serv_addr.sin_port = htons(PORT);
    
    // convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET,"127.0.0.1",&serv_addr.sin_addr) <= 0) {
       printf("\nInvalid address\n");
       return -1;
    }
    
    // connecting with the socket
    if(connect(sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) {
    	printf("\nconnection failed\n");
    	return -1;	  
    }
    
    // sending message to sever
    // param : socket, message, length 
    send(sock,hello,strlen(hello),0);
    printf("message sent from client\n");
    valread = read(sock,buffer,1024);
    printf("%s\n",buffer);
    return 0;
}
