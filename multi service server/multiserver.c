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

// receiving data from client socket
void *recvmg(void *client_sock) {


}

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
      
      if(bind(sock,(struct sockaddr *)&server_ip,sizeof(server_ip)) == -1) {
         printf("binding error!\n");
      }
      else {
         printf("server started...\n");
      } 
      
      // listen system call tells a socket that it should accepting incoming connections
    // 2nd param : maximum number of pending connections 
      if(listen(sock,20) == -1) {
         printf("listening failed for incoming connections");
      }

      // infinite loop for broadcasting messages
      while(1) {
     
          // accepting incoming connections from client
          if((client_socket == accept(sock,(struct sockaddr *)NULL,NULL)) < 0) {
              printf("failed  to accept connections\n");
          }
      
          pthread_mutex_lock(&mutex);
          clients[n] = client_socket;
          n++;
          // creating a thread for each client
          pthread_create(&recvt,NULL,(void *)recvmg,&client_socket);
      
          pthread_mutex_unlock(&mutex); 
      } 
}


