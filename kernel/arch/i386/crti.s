; x86 crti.s
format ELF
section '.init' executable 
public _init
_init:
	push ebp
	mov esp, ebp
	; gcc will nicely put the contents of crtbegin.o's .init section here
_init.end:

section '.fini' executable
public _fini
_fini:
	push ebp
	mov esp, ebp
	; gcc will nicely put the contents of crtbegin.o's .fini section here.
_fini.end:



