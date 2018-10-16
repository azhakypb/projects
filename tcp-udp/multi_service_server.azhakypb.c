#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/select.h>
#include <fcntl.h>


#define MAXPENDING 5
#define BUFFSIZE 1024
#define MAXLINE 100

       //secondary functions//
void Die(char *mess) { perror(mess); exit(1); }
int max(int a, int b){
  if(a>b){
    return a;
  }
  else{
    return b;
  }
}
     //main functions//
void respondTCP(int clientT); //function that operates with TCP




int main (int argc, char *argv[]){
  int listenfd, connectfd, udp, maxf, selred;
  pid_t childpid;
  fd_set fdset;
  
  struct sockaddr_in servAddress, cliAddress;
  socklen_t addrlen;
  
  //Creating listening TCP socket//
  if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
    Die("socket failed\n");
    exit(1);
  }
  bzero(&servAddress, sizeof(servAddress));
  servAddress.sin_family = AF_INET;
  servAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  servAddress.sin_port = htons(atoi(argv[1]));

  if(bind(listenfd, (struct sockaddr*)&servAddress, sizeof(servAddress)) < 0){
    Die("Failed to bind the server socket\n");
  }
  if(listen(listenfd, MAXPENDING) < 0){
      Die("Failed to listen on server socket\n");
  }
  //Creating UDP socket//
  if((udp = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
    Die("udp failed\n");
    exit(1);
  }
  if(bind(udp, (struct sockaddr*)&servAddress, sizeof(servAddress)) < 0){
    Die("Failed to bind the udp server socket\n");
  }
  FD_ZERO(&fdset);
  maxf = max(listenfd, udp) +1;

  //Look for connections and respond//
  while (1){
    FD_SET(listenfd, &fdset);
    FD_SET(udp, &fdset);
    selred = select(maxf, &fdset, NULL, NULL, NULL);

    if(FD_ISSET(listenfd, &fdset)){
      addrlen = sizeof(cliAddress);
      if((connectfd = accept(listenfd, (struct sockaddr*)&cliAddress, &addrlen))<0){
	Die("Failed to accept client connection\n");
      }
      if((childpid = fork()) == 0){
	close(listenfd);
	respondTCP(connectfd);
	close(connectfd);
	break;
      }
      close(connectfd);
    }
    if(FD_ISSET(udp, &fdset)){
      // do something//
      addrlen = sizeof(cliAddress);
      char buf[BUFFSIZE];
      memset(buf, '\0', sizeof(buf));
      recvfrom(udp, buf, sizeof(buf), 0, (struct sockaddr*)&cliAddress, &addrlen);
      sendto(udp, buf, sizeof(buf), 0, (struct sockaddr*)&cliAddress, &addrlen);
      //break;
    }
  }
  return 0;
}

void respondTCP(int clientT){
  char mess[BUFFSIZE];
  char path[BUFFSIZE];
  char buf[BUFFSIZE];
  char *line = NULL;
  int fd = 0;
  char h200[MAXLINE] = "HTTP/1.0 200 OK\r\n\r\n\0";
  char h403[MAXLINE] = "HTTP/1.0 403 Forbidden\r\n\r\n\0";
  char h404[MAXLINE] = "HTTP/1.0 404 Not Found\r\n\r\n\0";
  

  memset(mess, '\0', sizeof(mess));
  memset(path,'\0', sizeof(path));
  memset(buf, '\0', sizeof(buf));
  
  if(read(clientT, mess, sizeof(mess)) < 0){
    Die("recieve error\n");
  }
  else{
    line = strtok(mess," ");
    if(strncmp(line, "GET", 3) == 0){
      line = strtok(NULL, " ");
      strcpy(path, ".");
      strcat(path, line);
      if((fd = open(path, O_RDONLY)) !=-1){
	write(clientT, h200, sizeof(h200));
	while(read(fd, buf, BUFFSIZE) > 0){
	    write(clientT, buf, BUFFSIZE);
	    memset(buf, '\0', sizeof(buf));
	}
      }
      else if(strncmp(strerror(errno), "Permission denied", 17) == 0){
	write(clientT, h403, sizeof(h403));
      }
      else{
	write(clientT, h404, sizeof(h404));
      }
    }
  }
}
	
	  
	  
      
      
  
