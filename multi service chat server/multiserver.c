#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <netinet/in.h>
#include <unistd.h>

/*** globals ***/

// for multithreading
pthread_mutex_t mutex;
// maximum number of clients that can be handled
int clients[20];
int n=0;

void sendtoall(char *msg,int sock){
	int i;
	//printf("broadcast message : %s\n",msg);
	// thread locking using mutex to ensure synchronization of shared resources    
	pthread_mutex_lock(&mutex);
	for(i = 0; i < n; i++) {
		if(clients[i] != sock) {
		       //printf("broad - %s",msg);
		       // sending messages to all clients
			if(send(clients[i],msg,strlen(msg),0) < 0) {
				printf("failed to send\n");
				continue;
			}else {
			   	//printf("response sent from server\n\n");    
			}
		}
	}
	
	// unlocking the thread once the job is finished
	pthread_mutex_unlock(&mutex);
}

void header(int handler, int status) {
  char header[100] = {0};
  if (status == 0) {
    sprintf(header, "HTTP/1.0 200 OK\r\n\r\n");
  } else if (status == 1) {
    sprintf(header, "HTTP/1.0 403 Forbidden\r\n\r\n");
  } else {
    sprintf(header, "HTTP/1.0 404 Not Found\r\n\r\n");
  }
  //printf("response - %s\n",header);
  if(send(handler, header, strlen(header), 0) < 0) {
     printf("couldn't send status code\n"); 
  }
}

char *parse(char *req,int handler) {
  
  char buf[100]; 
  strcpy(buf,req);
  char *method;
 
   method = strtok(buf, " ");
   //printf("method : %s\n",method);
   //if (strcmp(method, "POST") != 0) return 1;
  
   int count = 1;
   while (method != NULL) {
        if (count == 1) {
         //printf("method is - %s\n", method);
         
         if(strcmp(method,"POST") != 0) {
             break;
         }
         
         method = strtok(NULL, " ");
         count++;
        }
        else if(count == 2) {
         //printf("header is - %s\n", method); 
         
         if(strcmp(method,"http://localhost:8080/multiserver.c") != 0) { 
            header(handler,2);
            break;
         }
            
         method = strtok(NULL, " ");
         count++;
        }
        else if(count == 3){
            //printf("protocol is : %s\n",method);
            
            if(strcmp(method,"HTTP/1.1") != 0) {
                header(handler,1);
                break;
            }
                
            method = strtok(NULL," ");
            count++;
        }
        else{
            header(handler,0);
            //printf("message is - %s\n",method);
            return (char *)method;
        }
    } 
    return (char *)method;
}


// receiving data from client socket
void *recvmg(void *client_sock){
	int sock = *((int *)client_sock);
	char msg[500];
	int len;
	char *pmsg;
	//char str1[5];
	//char str2[5];
	int c = 0;
	
	while((len = recv(sock,msg,500,0)) > 0) {
	        //printf("request received from client : ");
		msg[len] = '\0';
		//printf("msg %s",msg);
		pmsg = parse(msg,sock);
		// closing the socket if client disconnects
		if((strncmp(msg,"/exit",5)) == 0) {
		     clients[sock] = 0;
		     close(sock); 
		}
		//printf("parsed msg : %s",pmsg);
		sendtoall(pmsg,sock);
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
