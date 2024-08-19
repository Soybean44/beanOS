; x86 crti.s
section .init
global _init
global _init:function (_init.end - _init)
_init:
	push ebp
	mov esp, ebp
	; gcc will nicely put the contents of crtbegin.o's .init section here
.end:

section .fini
global _fini:function (_fini.end - _fini)
_fini:
	push ebp
	mov esp, ebp
	; gcc will nicely put the contents of crtbegin.o's .fini section here.
.end:



