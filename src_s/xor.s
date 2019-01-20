section .text
	global	_xor

_xor:
	push	rbp
	mov		rbp, rsp

	push	rdi ; str
	push	rsi ; str size
	push	rdx ; key
	push	rcx
	push	rbx

	xor		rcx, rcx

beg_loop:
	cmp		ecx, esi
	je		end

get_index:
	push	rdx

	xor		rax, rax
	xor		rdx, rdx
	xor		rbx, rbx

	mov		rax, rcx
	mov		rbx, 0x10
	idiv	rbx

	mov		rbx, rdx ; index
	pop		rdx

xor_char:
	xor		rax, rax

	mov		al, [rdi + rcx]
	xor		al, [rdx + rbx]
	mov		[rdi + rcx], al

end_loop:
	inc		ecx

	jmp		beg_loop

end:
	push	rbx
	push	rcx
	push	rdx
	push	rsi
	push	rdi

	leave
	ret
