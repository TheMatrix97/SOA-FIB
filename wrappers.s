# 1 "wrappers.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4

# 17 "/usr/include/stdc-predef.h" 3 4














































# 32 "<command-line>" 2
# 1 "wrappers.S"

# 1 "include/asm.h" 1













# 2 "wrappers.S" 2
.globl write;     .type write, @function;     .align 0;   write:
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
	
.globl gettime;     .type gettime, @function;     .align 0;   gettime:
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
