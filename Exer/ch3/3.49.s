	.file	"3.49.c"
	.text
	.globl	vframe
	.type	vframe, @function
vframe:
.LFB23:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	leaq	22(,%rdi,8), %rax
	andq	$-16, %rax
	subq	%rax, %rsp
	leaq	7(%rsp), %rax
	shrq	$3, %rax
	leaq	0(,%rax,8), %r8
	movq	%r8, %rcx
	leaq	-16(%rbp), %r9
	movq	%r9, 0(,%rax,8)
	movq	$1, -16(%rbp)
	jmp	.L2
.L3:
	movq	%rdx, (%rcx,%rax,8)
	addq	$1, -16(%rbp)
.L2:
	movq	-16(%rbp), %rax
	cmpq	%rax, %rdi
	jg	.L3
	movq	(%r8,%rsi,8), %rax
	movq	(%rax), %rax
	movq	-8(%rbp), %rsi
	xorq	%fs:40, %rsi
	jne	.L6
	leave
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
.L6:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE23:
	.size	vframe, .-vframe
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%d"
	.text
	.globl	main
	.type	main, @function
main:
.LFB24:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	subq	$16, %rsp
	.cfi_def_cfa_offset 32
	movq	%fs:40, %rax
	movq	%rax, 8(%rsp)
	xorl	%eax, %eax
	movq	%rsp, %rbx
	movq	%rbx, %rsi
	leaq	.LC0(%rip), %rdi
	call	__isoc99_scanf@PLT
	movq	%rbx, %rdx
	movl	$0, %esi
	movl	$5, %edi
	call	vframe
	movq	8(%rsp), %rcx
	xorq	%fs:40, %rcx
	jne	.L10
	movl	$0, %eax
	addq	$16, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
.L10:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE24:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0"
	.section	.note.GNU-stack,"",@progbits
