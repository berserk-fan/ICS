	.file	"3.40.c"
	.text
	.globl	fix_set_diagonal
	.type	fix_set_diagonal, @function
fix_set_diagonal:
.LFB0:
	.cfi_startproc
	leaq	1088(%rdi), %rax
	jmp	.L2
.L3:
	movl	%esi, (%rdi)
	addq	$68, %rdi
.L2:
	cmpq	%rax, %rdi
	jbe	.L3
	rep ret
	.cfi_endproc
.LFE0:
	.size	fix_set_diagonal, .-fix_set_diagonal
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	movl	$0, %eax
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0"
	.section	.note.GNU-stack,"",@progbits
