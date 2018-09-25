#include <asm.h>
ENTRY(write)
	SAVE_ALL
	movl 0(%esp),%ebx
	movl 4(%esp),%ecx
	movl 8(%esp),%edx
	movl $4,%eax
	sysenter $0x80
	movl %eax,18(%esp);
	EOI
	RESTORE_ALL
	iret
