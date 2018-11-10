#include <libc.h>
#include <sched.h>

char buff[24];

int pid;


long workload1(long n){
	int i;
	long sum = 0;
	for(i=0;i<n*n;i++){
		sum++;
	}
	return sum;
}

void print_one_stat(int st){
	char user[10];
	itoa(st,user);
	write(1,&user[0],strlen(user));
}

void print_stats(int pid){
	struct stats *st;
	get_stats(pid,st);
	char aux[10];
	itoa(pid,aux);
	write(1,&aux[0], strlen(aux));
	print_one_stat(st->user_ticks);
	print_one_stat(st->blocked_ticks);
	print_one_stat(st->ready_ticks);
	print_one_stat(st->system_ticks);

}

int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */
	set_sched_policy(0);
	int i = 0;
	int nhijos = 3;
	int n = 100;
	for(i = 0; i < nhijos; i++){
		int pid = fork();
		if(pid == 0){
			long res = workload1(n);
			char buff[10];
			itoa(res,buff);
			write(1,&buff[0],strlen(buff)+1);
			while(1){}
		}
	}

  while(1) { }
}
