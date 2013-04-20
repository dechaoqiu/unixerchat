#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <netinet/in.h>
#include <string.h>
#define DEST_IP "127.0.0.1"
#define SERVPORT 6666
#define MAXDATASIZE 1000 /* The maximum size of the transfered data each time */
#define BUFFSIZE MAXDATASIZE+1
int main(int argc, char ** argv){
	int sockfd,recvbytes;
	unsigned char buf[BUFFSIZE];/*The buffer to store the messages to be sent.*/
	struct sockaddr_in serv_addr;
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1){
		printf("Failed to create socket!\n");
		exit(1);
	}
	printf("Socket created!\n");
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(SERVPORT);
	serv_addr.sin_addr.s_addr=inet_addr(DEST_IP);
	bzero(&(serv_addr.sin_zero),8);
	printf("Ready to connect\n");
	if(connect(sockfd, (struct sockaddr *)&serv_addr,sizeof(struct sockaddr)) == -1) {
		printf("Failed to connect!\n");
		exit(1);
	}
	printf("Connected!\n");
	printf("buf: %d\n",sizeof(buf));
	memset(buf,0,BUFFSIZE);
	for(;;){
		/* Receive the reply from the server. */
		if((recvbytes=recv(sockfd,buf,MAXDATASIZE,0))==-1){
			printf("Failed to receive the reply from server!\n");
			exit(1);
		}
		printf("recvbytes: %d\n",recvbytes);
		//Print out the reply
		printf("The server said: %s\n",buf);	
		memset(buf,0,BUFFSIZE);
		/* Get user input from console. */
		if(fgets(buf,MAXDATASIZE,stdin)==NULL){
			exit(1);
		}
		/* Send data */
		if(send(sockfd, buf, sizeof(buf)+1, 0) == -1){
			printf("Failed to send data!\n");
		}

	}
	close(sockfd);
	printf("Connection closed!\n");
	return 0;
}
