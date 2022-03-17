#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

int main() {

      struct sockaddr_in server_ip;
      pthread recvt;

      // initializing socket       
      int sock = 0 , client_socket = 0;
      
      // address family used when setting up the socket 
      server_ip.sin_family = AF_INET;
      // pick a specific number. clients will need to know a port number to connect to eg : 8080
      server_ip.sin_port = htons(1234);
      // special address eg : 0.0.0.0
      server_ip.sin_addr.s_addr = inet_addr("127.0.0.1");
      
      // socket creation
      sock = socket(AF_INET, SOCK_STREAM,0);
      
      
      
      















}


