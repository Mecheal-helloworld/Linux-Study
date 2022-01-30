//server.c
//��Դ�����ںš�������᡿��վ��https://www.yanbinghu.com
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
    int    listenfd = 0;//����������
    int    connfd = 0; //������������
    socklen_t clilen;
    char recvMsg[MAXLINE] = {0};
    //�������Ϳͻ���socket��Ϣ
    struct sockaddr_in    cliaddr, servaddr;
    char ip[MAXLINE] = {0};

    //��ʼ�������socket��Ϣ
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;

    //�������ip�Ͷ˿ڣ�ʹ�������ip�Ͷ˿�
    if(3 == argc)
    {
        inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
        servaddr.sin_port = htons(atoi(argv[2]));
    }
    else
    {   
        //ʹ��Ĭ�ϵ�ip��port
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port        = htons(SERV_PORT);
    }
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == listenfd)
    {
        perror("socket error");
        return -1;
    }
    //��ָ��ip�Ͷ˿�
    if(0 != bind(listenfd,  (struct sockaddr *) &servaddr, sizeof(servaddr)))
    {
        perror("bind error");
        return -1;
    }

    printf("start server at %s:%d\n",inet_ntop(AF_INET,&servaddr.sin_addr,ip,MAXLINE),ntohs(servaddr.sin_port));
    listen(listenfd, 4);
  
    //�������Կͻ��˵�����
    clilen = sizeof(cliaddr);
    connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
    if(-1 == connfd)
    {
        perror("accept failed");
        return -1;
    }

    printf("connect from %s %d\n",inet_ntop(AF_INET,&cliaddr.sin_addr,ip,MAXLINE),ntohs(cliaddr.sin_port));
    
    //��ȡ�ͻ��˷��͵���Ϣ
    if(0 != read(connfd, recvMsg, MAXLINE))
    {
        printf("recv msg:%s\n",recvMsg);
    }

    //����ȡ����ԭ�ⲻ���ط��ͻ�ȥ
    write(connfd, recvMsg, MAXLINE);
    close(connfd);
    close(listenfd);
    return 0;
}
