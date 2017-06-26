	.file	"des.cpp"
	.text
	.p2align 4,,15
	.globl	_Z15DES_keyScheduleyPy
	.type	_Z15DES_keyScheduleyPy, @function
_Z15DES_keyScheduleyPy:
.LFB30:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	xorl	%ecx, %ecx
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$12, %esp
	.cfi_def_cfa_offset 32
	movl	32(%esp), %eax
	movl	36(%esp), %edx
	movl	40(%esp), %ebp
	.p2align 4,,7
	.p2align 3
.L2:
	movl	$0, 0(%ebp,%ecx,8)
	movl	$0, 4(%ebp,%ecx,8)
	addl	$1, %ecx
	cmpl	$16, %ecx
	jne	.L2
	movl	$15, %edi
	.p2align 4,,7
	.p2align 3
.L3:
	movl	%eax, %ebx
	movl	%edi, %esi
	andl	$15, %ebx
	movl	%ebp, %ecx
	sall	$4, %esi
	addl	%ebx, %esi
	sall	$3, %esi
	leal	KeysTab(%esi), %ebx
	addl	$KeysTab+32768, %esi
	movl	%edi, 4(%esp)
	.p2align 4,,7
	.p2align 3
.L4:
	movl	(%ebx), %edi
	orl	%edi, (%ecx)
	movl	4(%ebx), %edi
	addl	$2048, %ebx
	orl	%edi, 4(%ecx)
	addl	$8, %ecx
	cmpl	%esi, %ebx
	jne	.L4
	movl	4(%esp), %edi
	shrdl	$4, %edx, %eax
	shrl	$4, %edx
	subl	$1, %edi
	cmpl	$-1, %edi
	jne	.L3
	addl	$12, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE30:
	.size	_Z15DES_keyScheduleyPy, .-_Z15DES_keyScheduleyPy
	.p2align 4,,15
	.globl	_Z6DES_IPyPjS_
	.type	_Z6DES_IPyPjS_, @function
_Z6DES_IPyPjS_:
.LFB31:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	xorl	%eax, %eax
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	xorl	%edx, %edx
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movl	$7, %esi
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	movl	20(%esp), %ecx
	movl	24(%esp), %ebx
	.p2align 4,,7
	.p2align 3
.L11:
	movl	%esi, %ebp
	movzbl	%cl, %edi
	sall	$8, %ebp
	subl	$1, %esi
	addl	%ebp, %edi
	movl	IPTab(,%edi,8), %ebp
	shrdl	$8, %ebx, %ecx
	movl	IPTab+4(,%edi,8), %edi
	shrl	$8, %ebx
	orl	%eax, %ebp
	orl	%edx, %edi
	cmpl	$-1, %esi
	movl	%ebp, %eax
	movl	%edi, %edx
	jne	.L11
	movl	28(%esp), %edx
	movl	%edi, (%edx)
	movl	32(%esp), %edx
	movl	%ebp, (%edx)
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE31:
	.size	_Z6DES_IPyPjS_, .-_Z6DES_IPyPjS_
	.p2align 4,,15
	.globl	_Z5DES_Ej
	.type	_Z5DES_Ej, @function
_Z5DES_Ej:
.LFB32:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	movl	8(%esp), %ecx
	movl	%ecx, %edx
	movzbl	%cl, %ebx
	shrl	$24, %edx
	addl	$768, %ebx
	movl	Expan(,%edx,8), %eax
	movl	Expan+4(,%edx,8), %edx
	orl	Expan(,%ebx,8), %eax
	orl	Expan+4(,%ebx,8), %edx
	movzbl	%ch, %ebx
	shrl	$16, %ecx
	addl	$512, %ebx
	andl	$255, %ecx
	addl	$256, %ecx
	orl	Expan(,%ebx,8), %eax
	orl	Expan+4(,%ebx,8), %edx
	orl	Expan(,%ecx,8), %eax
	orl	Expan+4(,%ecx,8), %edx
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE32:
	.size	_Z5DES_Ej, .-_Z5DES_Ej
	.p2align 4,,15
	.globl	_Z8DES_Sboxy
	.type	_Z8DES_Sboxy, @function
_Z8DES_Sboxy:
.LFB33:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	movl	$7, %edx
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	xorl	%eax, %eax
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	16(%esp), %ecx
	movl	20(%esp), %ebx
	.p2align 4,,7
	.p2align 3
.L17:
	movl	%ecx, %esi
	movl	%edx, %edi
	andl	$63, %esi
	subl	$1, %edx
	sall	$6, %edi
	addl	%edi, %esi
	shrdl	$6, %ebx, %ecx
	orl	Sbox(,%esi,4), %eax
	shrl	$6, %ebx
	cmpl	$-1, %edx
	jne	.L17
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE33:
	.size	_Z8DES_Sboxy, .-_Z8DES_Sboxy
	.p2align 4,,15
	.globl	_Z5DES_Pj
	.type	_Z5DES_Pj, @function
_Z5DES_Pj:
.LFB34:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	xorl	%eax, %eax
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	xorl	%edx, %edx
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movl	$31, %edi
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	movl	20(%esp), %esi
	movl	$32, %ebx
	.p2align 4,,7
	.p2align 3
.L22:
	movl	%ebx, %ecx
	subb	p_table(%edx), %cl
	btl	%ecx, %esi
	jnc	.L21
	movl	%edi, %ecx
	movl	$1, %ebp
	subl	%edx, %ecx
	sall	%cl, %ebp
	orl	%ebp, %eax
.L21:
	addl	$1, %edx
	cmpl	$32, %edx
	jne	.L22
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE34:
	.size	_Z5DES_Pj, .-_Z5DES_Pj
	.p2align 4,,15
	.globl	_Z6DES_FPjj
	.type	_Z6DES_FPjj, @function
_Z6DES_FPjj:
.LFB35:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	xorl	%eax, %eax
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	xorl	%edx, %edx
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movl	$3, %esi
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$12, %esp
	.cfi_def_cfa_offset 32
	movl	32(%esp), %edi
	movl	36(%esp), %ebx
	movl	%eax, (%esp)
	movl	%edx, 4(%esp)
	movl	%edi, %ecx
.L29:
	movl	%esi, %ebp
	movzbl	%cl, %eax
	sall	$8, %ebp
	movzbl	%bl, %edi
	addl	%eax, %ebp
	leal	4(%esi), %eax
	movl	FPTab(,%ebp,8), %edx
	subl	$1, %esi
	sall	$8, %eax
	addl	%eax, %edi
	movl	FPTab+4(,%ebp,8), %eax
	orl	FPTab(,%edi,8), %edx
	orl	FPTab+4(,%edi,8), %eax
	orl	(%esp), %edx
	orl	4(%esp), %eax
	shrl	$8, %ebx
	shrl	$8, %ecx
	cmpl	$-1, %esi
	movl	%edx, (%esp)
	movl	%eax, 4(%esp)
	jne	.L29
	movl	(%esp), %eax
	movl	4(%esp), %edx
	addl	$12, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE35:
	.size	_Z6DES_FPjj, .-_Z6DES_FPjj
	.p2align 4,,15
	.globl	_Z11DES_EncryptyPKy
	.type	_Z11DES_EncryptyPKy, @function
_Z11DES_EncryptyPKy:
.LFB36:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	xorl	%ebp, %ebp
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	xorl	%edi, %edi
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movl	$7, %ecx
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	movl	%edi, %esi
	subl	$44, %esp
	.cfi_def_cfa_offset 64
	movl	%ebp, %edi
	movl	64(%esp), %eax
	movl	68(%esp), %edx
	.p2align 4,,7
	.p2align 3
.L33:
	movl	%ecx, %ebp
	movzbl	%al, %ebx
	sall	$8, %ebp
	subl	$1, %ecx
	addl	%ebp, %ebx
	movl	IPTab(,%ebx,8), %ebp
	shrdl	$8, %edx, %eax
	movl	IPTab+4(,%ebx,8), %ebx
	shrl	$8, %edx
	orl	%esi, %ebp
	orl	%edi, %ebx
	cmpl	$-1, %ecx
	movl	%ebp, %esi
	movl	%ebx, %edi
	jne	.L33
	movl	%ebx, 32(%esp)
	movl	$31, %esi
	movl	$32, %ebx
	movl	%ebp, 28(%esp)
	movl	$0, 8(%esp)
	.p2align 4,,7
	.p2align 3
.L37:
	movl	28(%esp), %eax
	movl	%eax, (%esp)
	call	_Z5DES_Ej
	movl	8(%esp), %edi
	movl	72(%esp), %ecx
	movl	%ebx, 36(%esp)
	xorl	(%ecx,%edi,8), %eax
	xorl	4(%ecx,%edi,8), %edx
	xorl	%edi, %edi
	movl	%eax, 16(%esp)
	movl	16(%esp), %ecx
	movl	$7, %eax
	movl	%edx, 20(%esp)
	movl	20(%esp), %ebx
	.p2align 4,,7
	.p2align 3
.L34:
	movl	%ecx, %edx
	movl	%eax, %ebp
	andl	$63, %edx
	subl	$1, %eax
	sall	$6, %ebp
	addl	%ebp, %edx
	shrdl	$6, %ebx, %ecx
	orl	Sbox(,%edx,4), %edi
	shrl	$6, %ebx
	cmpl	$-1, %eax
	jne	.L34
	movl	%ebx, 20(%esp)
	movl	36(%esp), %ebx
	xorl	%ebp, %ebp
	movl	%ecx, 16(%esp)
	xorl	%eax, %eax
	.p2align 4,,7
	.p2align 3
.L36:
	movl	%ebx, %edx
	subb	p_table(%eax), %dl
	btl	%edx, %edi
	jnc	.L35
	movl	%esi, %ecx
	movl	$1, %edx
	subl	%eax, %ecx
	sall	%cl, %edx
	orl	%edx, %ebp
.L35:
	addl	$1, %eax
	cmpl	$32, %eax
	jne	.L36
	addl	$1, 8(%esp)
	movl	28(%esp), %edi
	xorl	32(%esp), %ebp
	cmpl	$16, 8(%esp)
	movl	%edi, 32(%esp)
	je	.L48
	movl	%ebp, 28(%esp)
	jmp	.L37
.L48:
	xorl	%eax, %eax
	xorl	%edx, %edx
	movl	$3, %esi
	movl	%edi, %ecx
	movl	%eax, 8(%esp)
	movl	%ebp, %ebx
	movl	%edx, 12(%esp)
.L38:
	movl	%esi, %ebp
	movzbl	%bl, %eax
	sall	$8, %ebp
	movzbl	%cl, %edi
	addl	%eax, %ebp
	leal	4(%esi), %eax
	movl	FPTab(,%ebp,8), %edx
	subl	$1, %esi
	sall	$8, %eax
	addl	%eax, %edi
	movl	FPTab+4(,%ebp,8), %eax
	orl	FPTab(,%edi,8), %edx
	orl	FPTab+4(,%edi,8), %eax
	orl	8(%esp), %edx
	orl	12(%esp), %eax
	shrl	$8, %ecx
	shrl	$8, %ebx
	cmpl	$-1, %esi
	movl	%edx, 8(%esp)
	movl	%eax, 12(%esp)
	jne	.L38
	movl	8(%esp), %eax
	movl	12(%esp), %edx
	addl	$44, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE36:
	.size	_Z11DES_EncryptyPKy, .-_Z11DES_EncryptyPKy
	.globl	p_table
	.data
	.align 32
	.type	p_table, @object
	.size	p_table, 32
p_table:
	.byte	16
	.byte	7
	.byte	20
	.byte	21
	.byte	29
	.byte	12
	.byte	28
	.byte	17
	.byte	1
	.byte	15
	.byte	23
	.byte	26
	.byte	5
	.byte	18
	.byte	31
	.byte	10
	.byte	2
	.byte	8
	.byte	24
	.byte	14
	.byte	32
	.byte	27
	.byte	3
	.byte	9
	.byte	19
	.byte	13
	.byte	30
	.byte	6
	.byte	22
	.byte	11
	.byte	4
	.byte	25
	.ident	"GCC: (Ubuntu/Linaro 4.7.3-1ubuntu1) 4.7.3"
	.section	.note.GNU-stack,"",@progbits
