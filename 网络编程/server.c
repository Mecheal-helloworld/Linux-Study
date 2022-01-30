//server.c
//来源：公众号【编程珠玑】网站：https://www.yanbinghu.com
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include <arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#define SERV_PORT 1234
#define MAXLINE 128
int main(int argc, char **argv)
{
    int    listenfd = 0;//监听描述符
    int    connfd = 0; //已连接描述符
    socklen_t clilen;
    char recvMsg[MAXLINE] = {0};
    //服务器和客户端socket信息
    struct sockaddr_in    cliaddr, servaddr;
    char ip[MAXLINE] = {0};

    //初始化服务端socket信息
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;

    //如果输入ip和端口，使用输入的ip和端口
    if(3 == argc)
    {
        inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
        servaddr.sin_port = htons(atoi(argv[2]));
    }
    else
    {   
        //使用默认的ip和port
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port        = htons(SERV_PORT);
    }
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == listenfd)
    {
        perror("socket error");
        return -1;
    }
    //绑定指定ip和端口
    if(0 != bind(listenfd,  (struct sockaddr *) &servaddr, sizeof(servaddr)))
    {
        perror("bind error");
        return -1;
    }

    printf("start server at %s:%d\n",inet_ntop(AF_INET,&servaddr.sin_addr,ip,MAXLINE),ntohs(servaddr.sin_port));
    listen(listenfd, 4);
  
    //处理来自客户端的连接
    clilen = sizeof(cliaddr);
    connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
    if(-1 == connfd)
    {
        perror("accept failed");
        return -1;
    }

    printf("connect from %s %d\n",inet_ntop(AF_INET,&cliaddr.sin_addr,ip,MAXLINE),ntohs(cliaddr.sin_port));
    
    //读取客户端发送的消息
    if(0 != read(connfd, recvMsg, MAXLINE))
    {
        printf("recv msg:%s\n",recvMsg);
    }

    //将读取内容原封不动地发送回去
    write(connfd, recvMsg, MAXLINE);
    close(connfd);
    close(listenfd);
    return 0;
}
