	.file	"3.43.c"
	.text
	.globl	get
	.type	get, @function
get:
.LFB0:
	.cfi_startproc
	movq	8(%rdi), %rax
	movzbl	(%rax), %eax
	movb	%al, (%rsi)
	ret
	.cfi_endproc
.LFE0:
	.size	get, .-get
	.ident	"GCC: (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0"
	.section	.note.GNU-stack,"",@progbits
