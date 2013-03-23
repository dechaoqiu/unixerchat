#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <netinet/in.h>
#define SERVPORT 6666 /*服务器监听端口号 */
#define BACKLOG 10 /* 最大同时连接请求数 */
#define MAXDATASIZE 1000
#define BUFFSIZE MAXDATASIZE+1
int main(int argc,char ** argv)
{
	int sockfd,client_fd; /*sock_fd：监听socket；client_fd：数据传输socket */
	struct sockaddr_in my_addr; /* 本机地址信息 */
	struct sockaddr_in remote_addr; /* 客户端地址信息 */
	unsigned char buf[MAXDATASIZE];
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("socket创建出错！\n");
		exit(1);
	}
	printf("socket创建成功！\n");
	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(SERVPORT);
	my_addr.sin_addr.s_addr=INADDR_ANY;
	//bzero(&(my_addr.sin_zero),8);
	if(bind(sockfd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr))==-1){
		printf("bind出错！\n");
		exit(1);
	}
	printf("bind成功！\n");
	if(listen(sockfd,BACKLOG)==-1){
		printf("listen出错！\n");
		exit(1);
	}
	printf("listen成功！\n");
	while(1){
		int sin_size = sizeof(struct sockaddr_in);
		if((client_fd = accept(sockfd,(struct sockaddr *)&remote_addr,&sin_size)) == -1) {
			printf("accept出错\n");
			continue;
		}
		//printf("received a connection from client %s\n", inet_ntoa(remote_addr.sin_addr));
		printf("获取到了来自客户端的连接请求!\n");
	//	printf("xx:%d:\n",strlen("你好,服务器接收到了您的连接请求,连接成功!"));
		if (!fork()){ /* 子进程代码段 */
			printf("子进程创建完成,send开始！");
			if(send(client_fd, "你好,服务器接收到了您的连接请求,连接成功!\n", 61, 0) == -1){
				printf("send出错！\n");
			}
			//printf("send成功！\n");
			int recvbytes;
			//printf("开始获取客户端的输入\n");
			memset(buf,0,BUFFSIZE);
			if(recvbytes=recv(client_fd,buf,MAXDATASIZE,0)==-1){
				printf("recv出错\n");
				exit(1);
			}
			printf("客户端说:%s size: %d\n",buf,strlen(buf));
			memset(buf,0,BUFFSIZE);
			close(client_fd);
			exit(0);
		}
		printf("父进程\n");
		close(client_fd);
	}
	return 0;
}
