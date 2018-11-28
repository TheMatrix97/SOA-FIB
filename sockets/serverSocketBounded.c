#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#define MAX_CHILDS 10

int num_childs;

void doServiceFork(int fd){
	num_childs++;
	if(fork() == 0){
		doService(fd);
		exit(0);
	}
	close(fd);
}

doService(int fd) {
int i = 0;
char buff[80];
char buff2[80];
int ret;
int socket_fd = (int) fd;

	ret = read(socket_fd, buff, sizeof(buff));
	while(ret > 0) {
		buff[ret]='\0';
		sprintf(buff2, "Server [%d] received: %s\n", getpid(), buff);
		write(1, buff2, strlen(buff2));
		ret = write(fd, "caracola ", 8);
		if (ret < 0) {
			perror ("Error writing to socket");
			exit(1);
		}
		ret = read(socket_fd, buff, sizeof(buff));
	}
	if (ret < 0) {
			perror ("Error reading from socket");

	}
	sprintf(buff2, "Server [%d] ends service\n", getpid());
	write(1, buff2, strlen(buff2));

}

void trat_sigchld(int signum){
	printf("Ha muerto un hijo\n");
	num_childs--;
}

main (int argc, char *argv[])
{
  int socketFD;
  int connectionFD;
  char buffer[80];
  int ret;
  int port;
  num_childs = 0;
  signal(SIGCHLD,trat_sigchld);

  if (argc != 2)
    {
      strcpy (buffer, "Usage: ServerSocket PortNumber\n");
      write (2, buffer, strlen (buffer));
      exit (1);
    }

  port = atoi(argv[1]);
  socketFD = createServerSocket (port);
  if (socketFD < 0)
    {
      perror ("Error creating socket\n");
      exit (1);
    }

  while (1) {
  	if(num_childs < MAX_CHILDS){
  		printf("Entro\n");
		connectionFD = acceptNewConnections (socketFD);
		if (connectionFD < 0){
			perror ("Error establishing connection \n");
			deleteSocket(socketFD);
			exit (1);
		}
		doServiceFork(connectionFD);
	}else{
		printf("Se ha alcanzado el max de hijos\n");
	}
  }

}
