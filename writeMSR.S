#include <asm.h>
ENTRY(writeMSR)
	SAVE_ALL
	movl %ebx, %eax
	movl $0, %edx
	WRMSR
	RESTORE_ALL
	ret
	
