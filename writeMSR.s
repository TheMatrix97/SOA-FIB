# 1 "writeMSR.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4

# 17 "/usr/include/stdc-predef.h" 3 4














































# 32 "<command-line>" 2
# 1 "writeMSR.S"

# 1 "include/asm.h" 1













# 2 "writeMSR.S" 2
.globl writeMSR;     .type writeMSR, @function;     .align 0;   writeMSR:
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
	
