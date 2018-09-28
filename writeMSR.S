#include <asm.h>
ENTRY(writeMSR)
	push %ebp
	movl %esp, %ebp
	push %edx
	push %ecx
	movl 8(%ebp),%eax
	movl 12(%ebp),%ecx
	movl $0, %edx
	WRMSR
	pop %ecx
	pop %edx
	pop %ebp
	ret
	
