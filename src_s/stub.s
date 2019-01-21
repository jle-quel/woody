entry:
	push	rax
    push    rdi
    push    rsi
    push    rdx
	push	rcx
	push	rbx

init_payload:
	call	payload
	db		"...WOODY...", 0xa

payload:
	mov		rdi, 0x1
	pop		rsi
	mov		rdx, 0xc

	mov		rax, 0x1
	syscall

init_decrypter:
	xor		rdx, rdx
	xor		rcx, rcx

	lea		rdi, [ rel entry ]		; section_text 
	mov		rsi, 0x42				; section_text size
	mov		rdx, qword '12345678'	; 8 last key bytes
	push	rdx
	mov		rdx, qword '12345678'	; 8 first key bytes
	push	rdx

beg_loop:
	cmp		ecx, esi
	je		end_decrypter

get_index:
	xor		rax, rax
	xor		rdx, rdx
	xor		rbx, rbx

	mov		rax, rcx
	mov		rbx, 0x10
	idiv	rbx

	mov		rbx, rdx				; modulo

xor_char:
	xor		rax, rax

	mov		al, [rdi + rcx]
	xor		al, [rsp + rbx]
	mov		[rdi + rcx], al

end_loop:
	inc		ecx

	jmp		beg_loop
	
end_decrypter:
	pop		rax
	pop		rax

	pop		rbx
	pop		rcx
	pop		rdx
	pop		rsi
	pop		rdi
	pop		rax

	jmp		0xcafebabe				; p_vaddr section text

