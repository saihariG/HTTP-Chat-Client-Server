#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<string.h>

/** Programming with TCP/IP sockets
  *  1. Creating the socket
  *  2. Identifying / Naming the socket
  *  3. Waiting for an incoming connection
  *  4. Sending and receiving messages
  *  5. Closing the socket
  */
  
// default port address 
#define PORT 8080

/* 
  struct sockaddr_in 
  { 
    __uint8_t         sin_len; 
    sa_family_t       sin_family; 
    in_port_t         sin_port; 
    struct in_addr    sin_addr; 
    char              sin_zero[8]; 
  };

*/

int main(int argc, char const *argv[])
{
    int server_fd, new_socket; 
    long valread;
    // defined in the header netinet/in.h
    struct sockaddr_in address;
    int addrlen = sizeof(address);
 
    /*** HTTP header to make server work ***/
    
    // 1. HTTP/1.1 200 ok (version, status code, message)
    // 2. Content-Type : type of message server is sending
    // 3. Content-Length : number of bytes server is sending
    
    //int len = 23;     
    
    char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 23\n\nHello World From Server";
    
    /*
    printf("Enter Message to send : ");
    char ch;
    scanf("%s",&ch);
    char *hello = &ch; */
    
   
    /*** 1. Creating a Socket ***/    
    
    // socket system call
    // param : domain , type , protocol
    // domain : communication domain in which the socket should be created
    // type :  SOCK_STREAM (virtual circuit service) 
    // protocol : it is zero since there’s only one form of virtual circuit service
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    
    /*** 2. Identifying/Naming a Socket 
         i.e assigning a transport address to the socket (a port number in IP networking) 
     **/
    
    // address family used when setting up the socket 
    address.sin_family = AF_INET;
    // machines IP address 
    // special address 0.0.0.0, defined by the symbolic constant INADDR_ANY.
    address.sin_addr.s_addr = INADDR_ANY;
    // pick a specific number. clients will need to know a port number to connect to eg : 8080
    address.sin_port = htons( PORT );
    // htons converts a short integer(port) to a network representation  
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    
    /*** 3. Waiting for an incoming connection ***/
    
    // listen system call tells a socket that it should accepting incoming connections
    // 2nd param : maximum number of pending connections 
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    
    while(1) {
        printf("\n------Waiting for new connection------\n\n");
        
        
        if((new_socket = accept(server_fd,(struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
           perror("In accept");
           exit(EXIT_FAILURE);
        }
        
        /*** 4. sending & receiving messages ***/
        
        char buffer[30000] = {0};
        valread = read(new_socket,buffer,30000);
        printf("%s\n",buffer);
        write(new_socket,hello,strlen(hello));
        printf("------message sent from server------");
        // closing a socket once done communicating
        close(new_socket);
    }
    return 0;
}    

