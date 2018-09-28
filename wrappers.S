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
	movl $4,%eax
	sysenter
	pop %edx
	pop %ecx
	pop %ebx
	pop %ebp
	ret
