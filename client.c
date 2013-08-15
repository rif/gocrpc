#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define MAXRCVLEN 4096
#define SERVER_ADDR "127.0.0.1"
#define PORTNUM 1234

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, n;
    struct sockaddr_in serv_addr;

    char buffer[MAXRCVLEN];
    char msg[] = "{\"params\": [{\"A\": 2, \"B\": 3}], \"id\": 1, \"method\": \"Arith.Multiply\"}";
    
    // socket init and connect
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    serv_addr.sin_port = htons(PORTNUM);
 
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    // writing the call
    n = write(sockfd, msg, strlen(msg));
    
    if (n < 0) 
         error("ERROR writing to socket");

    // reading the response
    bzero(buffer,MAXRCVLEN);
    n = read(sockfd, buffer, MAXRCVLEN);
    
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",buffer);

    close(sockfd);
    return 0;
}


