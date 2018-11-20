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
	char user[100];
	itoa(st,user);
	write(1,&user[0],strlen(user) + 1);
}

void print_stats(int pid){
	struct stats st;
	int a = get_stats(pid,&st);
	char aux[10];
	itoa(pid,aux);
	write(1,&aux[0], strlen(aux) + 1);
	print_one_stat(st.user_ticks);
	print_one_stat(st.blocked_ticks);
	print_one_stat(st.ready_ticks);
	print_one_stat(st.system_ticks);

}
 
void work1(){
	int i = 0;
	int nhijos = 3;
	int n = 10000;
	for(i = 0; i < nhijos; i++){
		int pid = fork();
		if(pid == 0){
			long res = workload1(n);
			/*char buff[10];
			itoa(res,buff);
			write(1,&buff[0],strlen(buff)+1);*/
			print_stats(getpid());
			exit(0);
		}
	}

	read(0,NULL,200);
	print_stats(0); //print idle
	exit(0);
}
   
void work2(){
	int i = 0;
	int nhijos = 3;
	for(i = 0; i < nhijos; i++){
		int pid = fork();
		if(pid == 0){
			read(0, NULL, 200);
			print_stats(getpid());
			exit(0);
		}
	}
	read(0, NULL, 750);
	print_stats(0); 
	exit(0);
}

void work4(){
	int i = 0;
	int nhijos = 3;
	for(i = 0; i < nhijos; i++){
		int pid = fork();
		if(pid == 0){
			if(i == 0) read(0, NULL, 950);
			else if(i == 1) workload1(10000);
			else{
				workload1(1000 * (i+5));
				read(0, NULL, 1200);
			}
			print_stats(getpid());
			exit(0);
		}
	}
	read(0, NULL, 750);
	print_stats(0); 
	exit(0);
} 
  
void work3(){
	int i = 0;
	int nhijos = 3;
	for(i = 0; i < nhijos; i++){
		int pid = fork();
		if(pid == 0){
			int n = 1000 * (i+7);
			workload1(n); 
			read(0,NULL, 200);
			print_stats(getpid());
			exit(0);
		}
	}
	read(0,NULL,1500);
	print_stats(0);
	exit(0);
}
int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */
	set_sched_policy(1);  
	//set_sched_policy(1); 
	//work1();
	//work2();
	//work3();
	work4();
	while(1) { }
}
