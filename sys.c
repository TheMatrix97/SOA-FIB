/*
 * sys.c - Syscalls implementation
 */
#include <devices.h>

#include <utils.h>

#include <io.h>

#include <mm.h>

#include <mm_address.h>

#include <sched.h>

#define LECTURA 0
#define ESCRIPTURA 1

int check_fd(int fd, int permissions)
{
  if (fd!=1) return -9; /*EBADF*/
  if (permissions!=ESCRIPTURA) return -13; /*EACCES*/
  return 0;
}

int sys_ni_syscall()
{
	return -38; /*ENOSYS*/
}

int sys_getpid()
{
	return current()->PID;
}

int sys_fork()
{
  int PID=-1;

  // creates the child process
  
  return PID;
}

void sys_exit()
{  
}


int sys_write(int fd, char * buffer, int size){
	int aux = check_fd(fd,ESCRIPTURA);
	if(aux != 0) return aux;
	if(buffer == NULL) return -59; //EBFONT (bad font)
	if(size < 0) return -5; //EIO (i/o error)
	return sys_write_console(buffer,size);
	
}

int sys_write_console (char* buffer,int size){
	int i;
	for(i=0;i<size; i++){
		printc(buffer[i]);
	}
	return size;
}
