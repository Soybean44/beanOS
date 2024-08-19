; x86 crtn.s
format ELF
section ".init" executable
	; gcc will nicely put the contents of crtend.o's .init section here.
	pop ebp
	ret

section ".fini" executable
	; gcc will nicely put the contents of crtend.o's .fini section here.
	pop ebp
	ret
