#include <asm.h>
ENTRY(write)
	push %ebp
	movl %esp, %ebp
	push %ebx
	push %ecx
	push %edx
	movl 0(%esp),%ebx
	movl 4(%esp),%ecx
	movl 8(%esp),%edx
	movl $4,%eax
	sysenter
	movl %eax,18(%esp);
	pop %edx
	pop %ecx
	pop %ebx
	pop %ebp
	ret
