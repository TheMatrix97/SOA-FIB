/*
 * sched.c - initializes struct for task 0 anda task 1
 */

#include <sched.h>
#include <mm.h>
#include <io.h>

struct list_head freequeue;
struct list_head readyqueue;
struct task_struct* idle_task;

union task_union task[NR_TASKS]
  __attribute__((__section__(".data.task")));

//#if 0
struct task_struct *list_head_to_task_struct(struct list_head *l)
{
  return list_entry( l, struct task_struct, list);
}
//#endif

extern struct list_head blocked;


/* get_DIR - Returns the Page Directory address for task 't' */
page_table_entry * get_DIR (struct task_struct *t) 
{
	return t->dir_pages_baseAddr;
}

/* get_PT - Returns the Page Table address for task 't' */
page_table_entry * get_PT (struct task_struct *t) 
{
	return (page_table_entry *)(((unsigned int)(t->dir_pages_baseAddr->bits.pbase_addr))<<12);
}


int allocate_DIR(struct task_struct *t) 
{
	int pos;

	pos = ((int)t-(int)task)/sizeof(union task_union);

	t->dir_pages_baseAddr = (page_table_entry*) &dir_pages[pos]; 

	return 1;
}

void cpu_idle(void)
{
	__asm__ __volatile__("sti": : :"memory");

	while(1)
	{
	;
	}
}

void init_idle (void)
{
	struct list_head *first = list_first(&freequeue); //obtenemos un taskunion
	struct task_struct *first_str = list_head_to_task_struct(first);
	first_str->PID = 0;
	allocate_DIR(first_str);
	union task_union ctx;
	ctx.task = *first_str;
	task[0] = ctx;
	idle_task = first_str;
	ctx.stack[KERNEL_STACK_SIZE - 1] = (unsigned long) &cpu_idle;
	ctx.stack[KERNEL_STACK_SIZE - 2] = 0;
	first_str->ebp_pos = KERNEL_STACK_SIZE - 2;
}

void init_task1(void)
{
}


void init_sched(){
	int i;
	INIT_LIST_HEAD(&freequeue); //init freequeue
	for(i = 0; i < NR_TASKS; i++) list_add_tail(&task[i].task.list, &freequeue); //add procs a freeque
	INIT_LIST_HEAD(&readyqueue); //ini ready empty queue

}

struct task_struct* current()
{
  int ret_value;
  
  __asm__ __volatile__(
  	"movl %%esp, %0"
	: "=g" (ret_value)
  );
  return (struct task_struct*)(ret_value&0xfffff000);
}

