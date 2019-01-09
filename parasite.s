;; nasm -f elf64 parasite.s && ld -m elf_x86_64 parasite.o&& objdump -d a.out |
;; grep '[0-9a-f]:'|grep -v 'file'|cut -f2 -d:|cut -f1-6 -d' '|tr -s ' '|tr '\t' ' '|
;; sed 's/ $//g'|sed 's/ /\\x/g'|paste -d '' -s |sed 's/^/"/'|sed 's/$/"/g'

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

	jmp		L1