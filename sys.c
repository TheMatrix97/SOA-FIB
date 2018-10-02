/*
 * sys.c - Syscalls implementation
 */
#include <devices.h>

#include <utils.h>

#include <io.h>

#include <mm.h>

#include <mm_address.h>

#include <sched.h>

#include <errno.h>

#define LECTURA 0
#define ESCRIPTURA 1


int check_fd(int fd, int permissions)
{
  if (fd!=1) return -EBADF; /*EBADF*/
  if (permissions!=ESCRIPTURA) return -EACCES; /*EACCES*/
  return 0;
}

int sys_ni_syscall()
{
	return -ENOSYS; /*ENOSYS*/
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
	char *sysbuff;
	if(copy_from_user(&buffer,&sysbuff,sizeof(buffer)) < 0) return -EIO;
	if(aux >= 0){ //fd nice!
		if(buffer == NULL){
			return -EBFONT;
			 //EBFONT (bad font)
		}
		if(size < 0){
			return -EIO; //EIO (i/o error)
		}
		return sys_write_console(sysbuff,size);
	}else{
		return aux;
	}	
}

int sys_gettime(){
	return zeos_ticks;
}
