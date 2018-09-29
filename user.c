#include <libc.h>
#include <io.h>

char buff[24];

int pid;

long inner(long n){
	int i;
	long suma = 0;
	for(i=0;i<n;i++) suma = suma + i;
	return suma;
}

long outer(long n){
	int i;
	long acum;
	acum = 0;
	for(i=0; i<n; i++) acum = acum + inner(i);
	return acum;
}

int add(int par1, int par2){
	return par1 + par2;
}

int add2(int par1, int par2);

int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */

  long count, acum;
  count = 75;
  acum = 0;
  acum = outer(count);
  acum = add2(count,acum);
  char aux[40] = "Hello world";
  int ret = write(1,&aux[0],sizeof(aux));
  while(1){}
  return 0;
}