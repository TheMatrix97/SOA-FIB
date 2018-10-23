#include <asm.h>
ENTRY(task_switch)
	push %ebp
	movl %esp, %ebp
	push %esi
	push %edi
	push %ebx
	push 8(%ebp)
	call inner_task_switch
	pop %ebx
	pop %ebx
	pop %edi
	pop %esi
	pop %ebp
	ret
	
ENTRY(change_context)
	pushl %ebp
	movl %esp,%ebp
	movl 8(%ebp), %eax
	movl 12(%ebp), %ebx
	movl %ebp, (%eax)
	popl %ebp
	movl (%ebx),%esp
	popl %ebp
	ret
