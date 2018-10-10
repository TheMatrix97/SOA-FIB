/*
 * sched.c - initializes struct for task 0 anda task 1
 */

#include <sched.h>
#include <mm.h>
#include <io.h>
#include <libc.h>

struct list_head freequeue;
struct list_head readyqueue;
struct task_struct *idle_task;

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
	list_del(first);
	struct task_struct *first_str = list_head_to_task_struct(first);
	first_str->PID = 0;
	allocate_DIR(first_str);
	union task_union *ctx = (union task_union*)first_str;
	ctx->stack[KERNEL_STACK_SIZE - 1] = (unsigned long) &cpu_idle; //dir del codigo a ejecutar por la nueva task
	ctx->stack[KERNEL_STACK_SIZE - 2] = 0; //valor del ebp al volver
	first_str->ebp_pos = (unsigned long)&ctx->stack[KERNEL_STACK_SIZE - 2]; //posicion del stack donde guardamos el ebp
	idle_task = first_str;
}

void init_task1(void)
{
	struct list_head *first = list_first(&freequeue); //obtenemos un taskunion
	list_del(first);
	struct task_struct *first_str = list_head_to_task_struct(first);
	first_str->PID = 1;
	allocate_DIR(first_str);
	union task_union ctx;
	ctx.task = *first_str; //insertem la task al task union del ctxd
	set_user_pages(first_str); //inicialitzar espai d'adreces del proc
	tss.esp0 = (DWord)(ctx.stack); //canviem el esp per a que apunti a la nova pila de proc
	set_cr3(first_str->dir_pages_baseAddr); //actualitzem el cr3 per a que apunti al directori del nou proc
}


void init_sched(){
	int i;
	INIT_LIST_HEAD(&freequeue); //init freequeue
	for(i = 0; i < NR_TASKS; i++) list_add_tail(&task[i].task.list, &freequeue); //add procs a freeque
	INIT_LIST_HEAD(&readyqueue); //ini ready empty queue

}

void inner_task_switch(union task_union *new){
	tss.esp0 = (int)&(new -> stack[KERNEL_STACK_SIZE]);
	set_cr3(new->task.dir_pages_baseAddr);
	switch_tasks(&current()->ebp_pos, new->task.ebp_pos);
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

