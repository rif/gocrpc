#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define MAXRCVLEN 4096
#define SERVER_ADDR "127.0.0.1"
#define PORTNUM 1234

int id = 1;

void error(const char *msg) {
    perror(msg);
    exit(0);
}

int initSocket(const char *addr, const int port){
  int sockfd;
  struct sockaddr_in serv_addr;
  // socket init and connect
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) 
    error("ERROR opening socket");
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(addr);
  serv_addr.sin_port = htons(port);
  
  if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
    error("ERROR connecting");
  return sockfd;
}

void rpcCall(const int sockfd, const char *method, const char *params, char *buf) {
  int n;
  sprintf(buf, "{\"id\": %d, \"method\": \"%s\", \"params\": [%s]}", id++, method, params);
  // writing the call
  n = write(sockfd, buf, strlen(buf));
  
  if (n < 0) 
    error("ERROR writing to socket");

  // reading the response
  bzero(buf,MAXRCVLEN);
  n = read(sockfd, buf, MAXRCVLEN);
  
  if (n < 0) 
    error("ERROR reading from socket");
}

int main(int argc, char *argv[]) {
  int sockfd;
  char response[MAXRCVLEN];
  
  sockfd = initSocket(SERVER_ADDR, PORTNUM);
  
  rpcCall(sockfd, "Arith.Multiply", "{\"A\": 2, \"B\": 3}", response);

  printf("%s\n", response);
  
  close(sockfd);
  return 0;
}


