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
  
  //get a new task struct for the process
  if(list_empty(&freequeue)) return -1;
  struct list_head *first = list_first(&freequeue); //obtenemos un taskunion
  list_del(first);
  struct task_struct *first_str = list_head_to_task_struct(first);
  union task_union *padre = (union task_union *) current(); //union del pare
  union task_union *son = padre;
  //Copiar data pare a fill
  copy_data(padre,son,sizeof(union task_union));
  allocate_DIR(first_str);
  
  //reserva de frames per data+stack
  int frames[NUM_PAG_DATA];
  int i;
  for(i = 0; i < NUM_PAG_DATA; i++){
	int frame_num = alloc_frame();
	if(frame_num == -1) return -1; //TODO liberar todos los frames
  }
  
  page_table_entry * parePT = getPT(current());
  page_table_entry * childPT = get_PT(first_str);
  //set paginas del hijo a frames del padre (codigo) -> shared entre padre hijo
  for(i = 0; i < NUM_PAG_CODE; ++i){
	  set_ss_pag(childPT, PAG_LOG_INIT_CODE+i, get_frame(parePT, PAG_LOG_INIT_CODE+i));
  }
  //set paginas hijo a los frames reservados
  for(i = 0; i < NUM_PAG_DATA; ++i){
	  set_ss_pag(childPT, PAG_LOG_INIT_DATA+i, frames[i]);
  }

  
  
  
  
  
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

