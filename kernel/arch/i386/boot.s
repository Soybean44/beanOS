format ELF 
; Declare constants for the multiboot header.
MALIGN   =  1 shl 0 
MINFO    =  1 shl 1 
MGFX     =  0

MBFLAGS  =  MALIGN or MINFO or MGFX          ; this is the Multiboot 'flag' field
MAGIC    =  0x1BADB002          ; 'magic number' lets bootloader find the header
CHECKSUM = -(MAGIC + MBFLAGS)   ; checksum of above, to prove we are multiboot

; Declare a multiboot header that marks the program as a kernel.
section '.multiboot' align 4
  dd MAGIC
  dd MBFLAGS
  dd CHECKSUM
  times 5 dd 0

  dd 0
  dd 800
  dd 600
  dd 32


; Allocate the initial stack.
section '.bss' writeable align 4 
stack_bottom:
rb 16384                            ; 16 KiB
stack_top:

; The kernel entry point.
section '.boot' executable
public _start
_start:
  ; map the initial page dir so that we have an upper half kernel
  mov eax, (initial_page_dir - 0xC0000000)
  mov cr3, eax

  ; turn on physical address extention
  mov ecx, cr4
  or ecx, 0x10
  mov cr4, ecx

  ; enable paging
  mov ecx, cr0
  or ecx, 0x80000000
  mov cr0, ecx
  
  jmp higher_half

section '.text' executable
higher_half:
  mov esp, stack_top
  push ebx
  xor ebp, ebp
  extrn kernel_main
  call kernel_main

halt:
  hlt
  jmp halt

; paging setup
section '.data' writable align 4096
public initial_page_dir
initial_page_dir:
  dd 10000011b
  times 767 dd 0

  dd (0 shl 22) or 10000011b
  dd (1 shl 22) or 10000011b
  dd (2 shl 22) or 10000011b
  dd (3 shl 22) or 10000011b
  times 252 dd 0
