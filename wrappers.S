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
	push %ebp
	//fake dynamic link para que el handler pueda acceder a los valores de esta pila
	movl %esp,%ebp
	sysenter
label2:
	pop %ebp
	addl $4, %esp
	pop %ebp
	ret
