L1:
    push    rdi
    push    rsi
    push    rdx

	call	L2
	db		"...WOODY...", 0xa

L2:
	mov		rdi, 0x1
	pop		rsi
	mov		rdx, 0xc

	mov		rax, 0x1
	syscall

	pop		rdx
    pop		rsi
    pop		rdi

	jmp		0x000000
