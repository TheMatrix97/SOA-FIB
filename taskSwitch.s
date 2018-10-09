# 1 "taskSwitch.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4

# 17 "/usr/include/stdc-predef.h" 3 4














































# 32 "<command-line>" 2
# 1 "taskSwitch.S"

# 1 "include/asm.h" 1













# 2 "taskSwitch.S" 2
.globl task_switch;     .type task_switch, @function;     .align 0;   task_switch:
	push %ebp
	movl %esp, %ebp
	push %esi
	push %edi
	push %ebx
	call inner_task_switch
	pop %ebx
	pop %edi
	pop %esi
	pop %ebp
	ret
