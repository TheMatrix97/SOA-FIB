/*
 * sched.c - initializes struct for task 0 anda task 1
 */

#include <sched.h>
#include <interrupt.h>
#include <mm.h>
#include <utils.h>

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

void scheduler(){
	update_sched_data_rr();
	if(needs_sched_rr() == 1){ //si ha de sortir de cpu
		update_process_state_rr(current(),&readyqueue);
		sched_next_rr();
	}
}

void sched_next_rr(){
	struct task_struct* aux;
	if(!list_empty(&readyqueue)){
		struct list_head *first = list_first(&readyqueue);
		list_del(first);
		aux = list_head_to_task_struct(first);
	}else aux = idle_task;
	aux->state = ST_RUN;
	aux->quantum = QUANTUM;
	if(aux != current()){
		int ticks = get_ticks();
		current()->sts.user_ticks += ticks - current()->sts.elapsed_total_ticks;
		current()->sts.elapsed_total_ticks = ticks; 
		task_switch((union task_union* )aux);
	}
}


void update_sched_data_rr(){
	current()->sts.remaining_ticks = current()->quantum - current()->sts.remaining_ticks;
	current()->quantum--;
}
int needs_sched_rr(){
	if(current()->quantum <= 0){
		return 1;
	} return 0;
}

void update_process_state_rr (struct task_struct* t, struct list_head *dst_queue){
	int elapsed_ticks = t->sts.elapsed_total_ticks;
	if(t->state == ST_READY){
		t->sts.ready_ticks += get_ticks() - elapsed_ticks;
		t->sts.elapsed_total_ticks = get_ticks();
		list_del(&t->list);
		t->state = ST_RUN;
		t->sts.total_trans++;
	}else if(t->state == ST_RUN){
		t->sts.system_ticks += get_ticks() - elapsed_ticks;
		list_add_tail(&t->list,&readyqueue);
		t->state = ST_READY;
		t->sts.total_trans++;
	}
	t->sts.elapsed_total_ticks = get_ticks();
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
	first_str->dir_pages_baseAddr = get_DIR(first_str);
	union task_union *ctx = (union task_union*)first_str;
	ctx->stack[KERNEL_STACK_SIZE - 1] = (unsigned long) &cpu_idle; //dir del codigo a ejecutar por la nueva task
	ctx->stack[KERNEL_STACK_SIZE - 2] = 0; //valor del ebp al volver
	first_str->ebp_pos = (unsigned long)&ctx->stack[KERNEL_STACK_SIZE - 2]; //posicion del stack donde guardamos el ebp
	idle_task = first_str;
	set_quantum(first_str, QUANTUM);

	//INIT STATS
	init_stats(idle_task);
}

void init_task1(void)
{
	struct list_head *first = list_first(&freequeue); //obtenemos un taskunion
	list_del(first);
	struct task_struct *first_str = list_head_to_task_struct(first);
	first_str->state = ST_RUN;
	first_str->PID = 1;
	allocate_DIR(first_str);
	union task_union *ctx = (union task_union*)first_str;
	set_user_pages(first_str); //inicialitzar espai d'adreces del proc
	tss.esp0 = (int)&(ctx->stack[KERNEL_STACK_SIZE]); //canviem el esp per a que apunti a la nova pila de proc //actualitzem el cr3 per a que apunti al directori del nou proc
	set_quantum(first_str, QUANTUM);
  	writeMSR((unsigned long)&(ctx->stack[KERNEL_STACK_SIZE]),0x175);
  	set_cr3(first_str->dir_pages_baseAddr);

  	init_stats(first_str);
}

void init_stats(struct task_struct* aux){
	//INIT STATS
	aux->sts.user_ticks = 0;
	aux->sts.system_ticks = 0;
	aux->sts.blocked_ticks = 0;
	aux->sts.ready_ticks = 0;
	aux->sts.elapsed_total_ticks = get_ticks();
	aux->sts.total_trans = 0;
	aux->sts.remaining_ticks = 0;
}


void init_sched(){
	int i;
	INIT_LIST_HEAD(&freequeue); //init freequeue
	for(i = 0; i < NR_TASKS; i++) list_add_tail(&task[i].task.list, &freequeue); //add procs a freeque
	INIT_LIST_HEAD(&readyqueue); //ini ready empty queue

}

void inner_task_switch(union task_union *new){
	set_cr3(new->task.dir_pages_baseAddr);
	tss.esp0 = (int)&(new -> stack[KERNEL_STACK_SIZE]);
	writeMSR((unsigned long)&(new->stack[KERNEL_STACK_SIZE]),0x175);
	change_context(&current()->ebp_pos, &new->task.ebp_pos);
}

int get_quantum (struct task_struct* t){
	return t->quantum;

}
void set_quantum (struct task_struct* t, int new_quantum){
	t->quantum = new_quantum;
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

void update_ticks_sys(){
	current()->sts.system_ticks += get_ticks() - current()->sts.elapsed_total_ticks;
	current()->sts.elapsed_total_ticks = get_ticks();
}
void update_ticks_user(){
	current()->sts.user_ticks += get_ticks() - current()->sts.elapsed_total_ticks;
	current()->sts.elapsed_total_ticks = get_ticks();
}

