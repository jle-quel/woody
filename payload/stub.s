entry:
    push    rdi
    push    rsi
    push    rdx
	push	r8
	push	r9

;init_decrypter:
;	xor		r8, r8
;	xor		r9, r9
;
;	lea		r8, [ rel loop ] ; p_vaddr segment text
;	lea		r9, [ rel loop ] ; p_vaddr payload
;
;loop:
;	cmp		r8, r9
;	je		init_payload
;
;	inc		r8
;
;	jmp loop


init_payload:
	call	payload
	db		"...WOODY...", 0xa

payload:
	mov		rdi, 0x1
	pop		rsi
	mov		rdx, 0xc

	mov		rax, 0x1
	syscall

end_payload:
	pop		r9
	pop		r8
	pop		rdx
    pop		rsi
    pop		rdi

	jmp		0xcafebabe ; old entry
