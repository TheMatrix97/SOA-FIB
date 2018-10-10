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
	
ENTRY(switch_tasks)
	movl 4(%esp), %eax
	movl %ebp, (%eax)
	movl 8(%esp),%esp
	popl %ebp
	ret
