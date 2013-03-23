#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <netinet/in.h>
#include <string.h>
#define DEST_IP "127.0.0.1"
#define SERVPORT 6666
#define MAXDATASIZE 1000 /*每次最大数据传输量 */
#define BUFFSIZE MAXDATASIZE+1
int main(int argc, char ** argv){
	int sockfd,recvbytes;
	unsigned char buf[BUFFSIZE];//发送缓冲区
	struct sockaddr_in serv_addr;
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1){
		printf("socket创建出错！\n");
		exit(1);
	}
	printf("socket创建成功！\n");
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(SERVPORT);
	serv_addr.sin_addr.s_addr=inet_addr(DEST_IP);
	bzero(&(serv_addr.sin_zero),8);
	printf("准备连接\n");
	if(connect(sockfd, (struct sockaddr *)&serv_addr,sizeof(struct sockaddr)) == -1) {
		printf("connect出错！\n");
		exit(1);
	}
	printf("connect成功！\n");
	printf("buf: %d\n",sizeof(buf));
	memset(buf,0,BUFFSIZE);
	for(;;){
		//获取服务器回复
		if((recvbytes=recv(sockfd,buf,MAXDATASIZE,0))==-1){
			printf("recv出错！\n");
			exit(1);
		}
		printf("recvbytes: %d\n",recvbytes);
		//打印服务器回复
		printf("服务器说: %s\n",buf);	
		memset(buf,0,BUFFSIZE);
		//获取用户输入
		if(fgets(buf,MAXDATASIZE,stdin)==NULL){
			exit(1);
		}
		//发送数据
		if(send(sockfd, buf, sizeof(buf)+1, 0) == -1){
			printf("send出错！\n");
		}
		//printf("send成功！\n");
	}
	close(sockfd);
	printf("连接关闭\n");
	return 0;
}
