#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthreaed.h>
#include <errno.h>


int main(int argc,char *argv[]) {

    pthread_t recvt;
    
    int len, sock;
    struct sockaddr_in server_ip;
    
    if(argc != 2) {
       fprintf(stderr,"USAGE: ./client <client name>"); 
       return 1;
    }	      

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













}
