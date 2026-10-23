	.file	"darray.c"
	.text
	.globl	int_cmp                         # -- Begin function int_cmp
	.p2align	4
	.type	int_cmp,@function
int_cmp:                                # @int_cmp
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rax
	movl	(%rax), %eax
	movq	-16(%rbp), %rcx
	subl	(%rcx), %eax
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end0:
	.size	int_cmp, .Lfunc_end0-int_cmp
	.cfi_endproc
                                        # -- End function
	.globl	main                            # -- Begin function main
	.p2align	4
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	andq	$-32, %rsp
	subq	$1216, %rsp                     # imm = 0x4C0
	movq	%fs:40, %rax
	movq	%rax, 1192(%rsp)
	leaq	160(%rsp), %rdi
	movq	%rdi, 24(%rsp)                  # 8-byte Spill
	xorl	%esi, %esi
	movl	%esi, 36(%rsp)                  # 4-byte Spill
	movl	$1024, %edx                     # imm = 0x400
	movq	%rdx, 16(%rsp)                  # 8-byte Spill
	callq	memset@PLT
	movq	16(%rsp), %rdi                  # 8-byte Reload
                                        # kill: def $rcx killed $rax
	movq	24(%rsp), %rax                  # 8-byte Reload
	movq	%rax, 136(%rsp)
	movq	$1024, 144(%rsp)                # imm = 0x400
	movq	$0, 152(%rsp)
	leaq	136(%rsp), %rax
	movq	%rax, 72(%rsp)
	callq	_Z14ark_make_arenaDU64_@PLT
	movq	%rax, 128(%rsp)
	movq	128(%rsp), %rax
	movq	%rax, 88(%rsp)
	movq	ark_arena_alloc@GOTPCREL(%rip), %rax
	movq	%rax, 96(%rsp)
	movq	$0, 104(%rsp)
	movq	$0, 112(%rsp)
	movb	$0, 120(%rsp)
	movl	$0, 124(%rsp)
	movl	$0, 121(%rsp)
	leaq	88(%rsp), %rdi
	movl	$512, %esi                      # imm = 0x200
	movl	$32, %ecx
	movq	%rcx, 40(%rsp)                  # 8-byte Spill
	movq	%rcx, %rdx
	callq	_Z24___ark__make_darray_fn__P23interface_ark_allocatorDU64_S1_S1_@PLT
	movq	%rax, %rcx
	movl	36(%rsp), %eax                  # 4-byte Reload
	movq	%rcx, 56(%rsp)
	movq	56(%rsp), %rcx
	movq	%rcx, 64(%rsp)
	movq	64(%rsp), %rcx
	movq	8(%rcx), %rsi
	leaq	.L.str(%rip), %rdi
                                        # kill: def $al killed $al killed $eax
	movb	%al, 55(%rsp)                   # 1-byte Spill
	callq	printf@PLT
	movq	40(%rsp), %rsi                  # 8-byte Reload
                                        # kill: def $ecx killed $eax
	movb	55(%rsp), %al                   # 1-byte Reload
	leaq	.L.str.1(%rip), %rdi
	callq	printf@PLT
	movq	40(%rsp), %rsi                  # 8-byte Reload
                                        # kill: def $ecx killed $eax
	movb	55(%rsp), %al                   # 1-byte Reload
	leaq	.L.str.2(%rip), %rdi
	callq	printf@PLT
	movl	$10, %edi
	callq	putchar@PLT
	leaq	128(%rsp), %rax
	movq	%rax, 80(%rsp)
	movq	80(%rsp), %rax
	movq	(%rax), %rdi
	callq	ark_destroy_arena@PLT
	movq	%fs:40, %rax
	movq	1192(%rsp), %rcx
	cmpq	%rcx, %rax
	jne	.LBB1_2
# %bb.1:
	xorl	%eax, %eax
	movq	%rbp, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.LBB1_2:
	.cfi_def_cfa %rbp, 16
	callq	__stack_chk_fail@PLT
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function
	.type	.L.str,@object                  # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"da cap: %zu\n"
	.size	.L.str, 13

	.type	.L.str.1,@object                # @.str.1
.L.str.1:
	.asciz	"da aligment: %zu\n"
	.size	.L.str.1, 18

	.type	.L.str.2,@object                # @.str.2
.L.str.2:
	.asciz	"vector aligment: %zu\n"
	.size	.L.str.2, 22

	.ident	"clang version 22.1.6"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym _Z14ark_make_arenaDU64_
	.addrsig_sym _Z24___ark__make_darray_fn__P23interface_ark_allocatorDU64_S1_S1_
	.addrsig_sym ark_arena_alloc
	.addrsig_sym printf
	.addrsig_sym putchar
	.addrsig_sym ark_destroy_arena
	.addrsig_sym __stack_chk_fail
