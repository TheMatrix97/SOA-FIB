#include <asm.h>
ENTRY(write)
	push %ebp
	movl %esp, %ebp
	push %ebx
	push %ecx
	push %edx
	movl 8(%ebp),%ebx
	movl 12(%ebp),%ecx
	movl 16(%ebp),%edx
	lea label,%eax
	//hacemos push de la direccion de retorno del sysenter
	push %eax 
	movl $4,%eax	
	//4 = write
	push %ebp
	//fake dynamic link para que el handler pueda acceder a los valores de esta pila
	movl %esp,%ebp
	sysenter
label:
	movl %eax, errno
	pop %ebp
	//sumamos 4 a la pila para eliminar la direccion de retorno
	//no hacemos pop eax pork nos cargamos el resultado de sysenter...
	addl $4, %esp	
	pop %edx
	pop %ecx
	pop %ebx
	pop %ebp
	ret
	
ENTRY(gettime)
	push %ebp
	movl %esp, %ebp
	lea label2,%eax
	//hacemos push de la direccion de retorno del sysenter
	push %eax 
	movl $10,%eax 
	//10 = gettime
	push %ebp
	//fake dynamic link para que el handler pueda acceder a los valores de esta pila
	movl %esp,%ebp
	sysenter
label2:
	pop %ebp
	addl $4, %esp
	pop %ebp
	ret
	
ENTRY(getpid)
	push %ebp
	movl %esp, %ebp
	lea label2pid,%eax
	//hacemos push de la direccion de retorno del sysenter
	push %eax 
	movl $20,%eax 
	//20 = get_pid
	push %ebp
	//fake dynamic link para que el handler pueda acceder a los valores de esta pila
	movl %esp,%ebp
	sysenter
label2pid:
	pop %ebp
	addl $4, %esp
	pop %ebp
	ret
	
ENTRY(fork)
	push %ebp
	movl %esp, %ebp
	lea labelFork,%eax
	//hacemos push de la direccion de retorno del sysenter
	push %eax 
	movl $2,%eax 
	//2 = fork
	push %ebp
	//fake dynamic link para que el handler pueda acceder a los valores de esta pila
	movl %esp,%ebp
	sysenter
labelFork:
	pop %ebp
	addl $4, %esp
	pop %ebp
	ret

ENTRY(exit)
	push %ebp
	movl %esp, %ebp
	lea labelExit,%eax
	//hacemos push de la direccion de retorno del sysenter
	push %eax 
	movl $1,%eax 
	//1 = exit
	push %ebp
	//fake dynamic link para que el handler pueda acceder a los valores de esta pila
	movl %esp,%ebp
	sysenter
labelExit:
	pop %ebp
	addl $4, %esp
	pop %ebp
	ret

ENTRY(get_stats)
	push %ebp
	movl %esp, %ebp
	lea labelgetStats,%eax
	//hacemos push de la direccion de retorno del sysenter
	push %eax 
	movl $35,%eax 
	//35 = get_stats
	push %ebp
	//fake dynamic link para que el handler pueda acceder a los valores de esta pila
	movl %esp,%ebp
	sysenter
labelgetStats:
	pop %ebp
	addl $4, %esp
	pop %ebp
	ret