#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <netinet/in.h>

#include <string.h>  /* To use the function strlen and memset */
#define SERVPORT 6666 /*The port that the server listens */
#define BACKLOG 10 /* The maxium number of connections */
#define MAXDATASIZE 1000
#define BUFFSIZE MAXDATASIZE+1
int main(int argc,char ** argv)
{
	int sockfd,client_fd; /*sock_fd：The socket that listens to, client_fd：Data transfermation socket */
	struct sockaddr_in my_addr; /* The local address information */
	struct sockaddr_in remote_addr; /* The client address information */
	unsigned char buf[MAXDATASIZE];
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("Failed to create socket!\n");
		exit(1);
	}
	printf("Socket created!\n");
	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(SERVPORT);
	my_addr.sin_addr.s_addr=INADDR_ANY;
	//bzero(&(my_addr.sin_zero),8);
	if(bind(sockfd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr))==-1){
		printf("Failed to bind!\n");
		exit(1);
	}
	printf("Bound successfully!\n");
	if(listen(sockfd,BACKLOG)==-1){
		printf("Failed to listen!\n");
		exit(1);
	}
	printf("Listen to port %d!\n", SERVPORT);
	while(1){
		int sin_size = sizeof(struct sockaddr_in);
		if((client_fd = accept(sockfd,(struct sockaddr *)&remote_addr,&sin_size)) == -1) {
			printf("Failed to accept connections!\n");
			continue;
		}
		//printf("received a connection from client %s\n", inet_ntoa(remote_addr.sin_addr));
		printf("Received the connection from client!\n");

		if (!fork()){ /* Sub thread code block */
			printf("Child process created! Start sending messages!");
			if(send(client_fd, "Hi, there, the server received your connection request, you have connected to the server successfully!\n", 61, 0) == -1){
				printf("Failed to send!\n");
			}

			int recvbytes;
			
			memset(buf,0,BUFFSIZE);
			if(recvbytes=recv(client_fd,buf,MAXDATASIZE,0)==-1){
				printf("Failed to receive!\n");
				exit(1);
			}
			printf("The client says:%s size: %d\n",buf,strlen(buf));
			memset(buf,0,BUFFSIZE);
			close(client_fd);
			exit(0);
		}
		printf("Parent process\n");
		close(client_fd);
	}
	return 0;
}
