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
	frames[i] = alloc_frame();
	if(frames[i] == -1) return -1; //TODO liberar todos los frames
  }
  
  page_table_entry * parePT = get_PT(current());
  page_table_entry * childPT = get_PT(first_str);
  
  //set paginas kernel (copia del padre)
  for(i = 0; i < NUM_PAG_KERNEL; i++){ //(kernel)
  	set_ss_pag(childPT, i, get_frame(parePT, i));
  }

  //set paginas del hijo a frames del padre (codigo) -> shared entre padre hijo
  for(i = 0; i < NUM_PAG_CODE; ++i){ //(code)
	  set_ss_pag(childPT, PAG_LOG_INIT_CODE+i, get_frame(parePT, PAG_LOG_INIT_CODE+i));
  }
  //set paginas hijo a los frames reservados
  for(i = 0; i < NUM_PAG_DATA; ++i){ //(data+stack)
	  set_ss_pag(childPT, PAG_LOG_INIT_DATA+i, frames[i]);
	  
  }
  //copiamos las paginas del padre al hijo
  // copy_data(parePT+PAG_LOG_INIT_DATA, childPT+PAG_LOG_INIT_DATA, NUM_PAG_DATA);
  //no podemos hacer copy directo ya que las @logicas del hijo i padre són las mismas i el padre tiene sus traducciones
  //crearemos traducciones en la TP(al final) del padre que apunten a los frames fisicos del hijo (vec frames)
  //una vez tengamos las traducciones podremos copiar de data padre a la nueva pag del padre
  unsigned long basepila = PAG_LOG_INIT_DATA + (NUM_PAG_DATA); //@logica base;
  for(i=0; i < NUM_PAG_DATA; i++){
  	unsigned long pag_nueva = basepila + i; //@ nueva pag
  	set_ss_pag(parePT, pag_nueva,frames[i]); //allocatamos data del hijo en el padre
  	unsigned long add_origen = (PAG_LOG_INIT_DATA + (i))*PAGE_SIZE; //data del padre pasamos a @log
  	unsigned long add_dest = pag_nueva*PAGE_SIZE; //pag nueva creada en el padre pasamos a @log
  	copy_data((void*)add_origen, (void*)add_dest, PAGE_SIZE); //copia una pagina
  	del_ss_pag(parePT, pag_nueva); //BORRAMOS la página creada*/
  }
  set_cr3(get_DIR(current())); //flush de tlb
  int pid_hijo = current()->PID + 200; //todos los procs del orden 200 serán hijos
  first_str->PID = pid_hijo;
  first_str->dir_pages_baseAddr = get_DIR(first_str);
  //KERNEL_STACK_SIZE-16 para llegar al ctx hijo pusheado en la pila
  //hay que modificar el valor de retorno %eax
  //son->stack[KERNEL_STACK_SIZE-18] = 
  //son->stack[KERNEL_STACK_SIZE-19] = 0; //somos el hijo, %eax = 0
  //ver diapo 54 tema4
  son->stack[KERNEL_STACK_SIZE-19] = 0; //ebp
  son->stack[KERNEL_STACK_SIZE-18] = (unsigned long) &ret_from_fork;//@ tornada
  first_str->ebp_pos = (unsigned long)&son->stack[KERNEL_STACK_SIZE - 18]; //@dir

  //push a la cua de ready
  list_add_tail(&son->task.list,&readyqueue);



  return first_str->PID;
}
int ret_from_fork(){
	return 0;
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

