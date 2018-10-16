#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define h_addr h_addr_list[0]

#define MAXLINE 100
int open_clientfd(char *servN, int port);

int main (int argc, char **argv){
  if(argc != 4){
    printf("Wrong input\n");
    exit(0);
  }
    
  int clientfd=0;
  int port=0;
  char *servName=NULL;
  char *pathname=NULL;
  char buf[MAXLINE] = {0};
  
  servName = argv[1];
  port = atoi(argv[2]);
  pathname = argv[3];

  clientfd = open_clientfd(servName, port);
  if(clientfd < 0){
    printf("Error opening connection\n");
    exit(0);
  }
  memset(buf, '\0', sizeof(buf));
  sprintf(buf, "GET %s HTTP/1.0\r\n\r\n", pathname);
  write(clientfd, buf, strlen(buf));
  //bzero(buf, sizeof(buf));
  
  while(read(clientfd, buf, MAXLINE) > 0){
    printf("%s", buf);
    memset(buf, '\0', sizeof(buf));
  }
  printf("\n");
  
  close(clientfd);
  return 0;
  //exit(0);
}
int open_clientfd(char *servN, int port){
  int client;
  struct hostent *hp;
  struct sockaddr_in serveraddr;

  if((client = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    printf("Error line 49\n");
    return -1;
  }

  if((hp = gethostbyname(servN)) ==NULL){
    printf("Error line 54\n");
    return -2;
  }
  bzero((char*)&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  bcopy((char*)hp->h_addr,
	(char*)&serveraddr.sin_addr.s_addr, hp->h_length);
  serveraddr.sin_port = htons(port);

  if(connect(client,(struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0){
    printf("Error line 64\n");
    return -1;
  }
  return client;
}
