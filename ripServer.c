#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
  int listenfd = 0,connfd = 0;

  struct sockaddr_in serv_addr;

  int numrv;  

  int bufsize=1024;        /* a 1K buffer */
  char *buffer=malloc(bufsize);

  pid_t childpid; /* variable to store the child's pid */
  int retval;     /* child process: user-provided return code */
  int status;     /* parent process: child's exit status */
  int match = 0;
  char * strArr;
  char * parmList[2];
  
  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  printf("socket retrieve success\n");

  memset(&serv_addr, '0', sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;    
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
  serv_addr.sin_port = htons(5000);    

  bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));

  if(listen(listenfd, 10) == -1){
    printf("Failed to listen\n");
    return -1;
  }

  while(1)
  {

    connfd = accept(listenfd, (struct sockaddr*)NULL ,NULL); // accept awaiting request

    recv(connfd, buffer, bufsize, 0);

    strArr = strtok(buffer, " ");

    if (strcmp("dvd", strArr) == 0) {
      parmList[0] = "-c";
      parmList[1] = "/usr/local/bin/dvd_trigger";
      parmList[2] = strtok(NULL, " ");
      parmList[3] = NULL;
      match = 1;
    }
    if ((strcmp("bluray", strArr) == 0) && match == 0) {
      parmList[0] = "-c";
      parmList[1] = "/usr/local/bin/bluray_trigger";
      parmList[2] = strtok(NULL, " ");
      parmList[3] = NULL;
      match = 1;
    }

    close(connfd);

    if (match > 0)
    {
      match = 0;
      childpid = fork();
      if (childpid == 0)
      {
        execv("/bin/sh", parmList);
      }
      else if (childpid < 0)
      {   // handle error
        printf("Child Error\n");
      }
    }

    sleep(1);
  }


  return 0;
}
