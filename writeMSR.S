#include <asm.h>
ENTRY(writeMSR)
	push %ebp
	movl %esp, %ebp
	pushl %edx
	movl %ebx, %eax
	movl $0, %edx
	WRMSR
	pop %edx
	pop %ebp
	ret
	
