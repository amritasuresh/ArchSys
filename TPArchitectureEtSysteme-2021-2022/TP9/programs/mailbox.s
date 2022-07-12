	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 10, 15, 6	sdk_version 10, 15, 6
	.globl	_routine                ## -- Begin function routine
	.p2align	4, 0x90
_routine:                               ## @routine
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movl	$0, -4(%rbp)
	movl	$0, -8(%rbp)
LBB0_1:                                 ## =>This Inner Loop Header: Depth=1
	cmpl	$1000000, -8(%rbp)      ## imm = 0xF4240
	jge	LBB0_4
## %bb.2:                               ##   in Loop: Header=BB0_1 Depth=1
	movl	_mails(%rip), %eax
	addl	$1, %eax
	movl	%eax, _mails(%rip)
	movl	-4(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -4(%rbp)
## %bb.3:                               ##   in Loop: Header=BB0_1 Depth=1
	movl	-8(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -8(%rbp)
	jmp	LBB0_1
LBB0_4:
	movl	-4(%rbp), %esi
	leaq	L_.str(%rip), %rdi
	movb	$0, %al
	callq	_printf
	xorl	%ecx, %ecx
	movl	%ecx, %edx
	movl	%eax, -12(%rbp)         ## 4-byte Spill
	movq	%rdx, %rax
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_main                   ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$64, %rsp
	xorl	%eax, %eax
	movl	%eax, %ecx
	leaq	_routine(%rip), %rdx
	movl	$0, -4(%rbp)
	movl	%edi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	leaq	-24(%rbp), %rdi
	movq	%rcx, %rsi
	callq	_pthread_create
	xorl	%r8d, %r8d
	movl	%r8d, %ecx
	leaq	_routine(%rip), %rdx
	leaq	-32(%rbp), %rdi
	movq	%rcx, %rsi
	movl	%eax, -36(%rbp)         ## 4-byte Spill
	callq	_pthread_create
	xorl	%r8d, %r8d
	movl	%r8d, %esi
	movq	-24(%rbp), %rdi
	movl	%eax, -40(%rbp)         ## 4-byte Spill
	callq	_pthread_join
	xorl	%r8d, %r8d
	movl	%r8d, %esi
	movq	-32(%rbp), %rdi
	movl	%eax, -44(%rbp)         ## 4-byte Spill
	callq	_pthread_join
	movl	_mails(%rip), %esi
	leaq	L_.str.1(%rip), %rdi
	movl	%eax, -48(%rbp)         ## 4-byte Spill
	movb	$0, %al
	callq	_printf
	xorl	%esi, %esi
	movl	%eax, -52(%rbp)         ## 4-byte Spill
	movl	%esi, %eax
	addq	$64, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_mails                  ## @mails
.zerofill __DATA,__common,_mails,4,2
	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"t1 = %d\n"

L_.str.1:                               ## @.str.1
	.asciz	"Number of mails: %d\n"

.subsections_via_symbols
