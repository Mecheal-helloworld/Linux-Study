#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include <arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#define MAXLINE 128
int main(int argc, char **argv)
{
    int    sockfd;  //connection descriper
    struct sockaddr_in    servaddr;//socket struct
    char sendMsg[MAXLINE] = {0};
    char recvMsg[MAXLINE] = {0};
    
    //check parameter number
    if (argc < 2)
    {
        printf("usage: ./client ip port\n");
        return -1;
    }
    //initializer struct
    bzero(&servaddr, sizeof(servaddr));
    
    //set proocol family
    servaddr.sin_family = AF_INET;
    //the first parameter is ip address,need to transform ip address to type of sin_addr
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    //the second  is port
    servaddr.sin_port = htons(atoi(argv[2]));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sockfd)
    {
        perror("socket error");
        return -1;
    }

    //connect to servlet,if not 0,connect failed
    if(0 != connect(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)))
    {
        perror("connect failed");
        return -1;
    }
    
    //read message from console
    if(NULL !=fgets(sendMsg,MAXLINE,stdin))
    {        
        write(sockfd, sendMsg, strlen(sendMsg));
    }
    if(0 != read(sockfd, recvMsg, MAXLINE))
    {
        printf("recv msg:%s\n",recvMsg);
    }
    close(sockfd);
    return 0;
}
