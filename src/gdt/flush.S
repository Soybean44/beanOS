format ELF64

section '.text' executable

public setGdt
setGdt:
  mov rax, [rsp+8]   ; Use RSP instead of ESP and RAX instead of EAX
  lgdt [rax]         ; Load the GDT
  ret

public reloadSegments
reloadSegments:
  mov eax, 0x10      ; Load 64-bit data segment selector (e.g., 0x10)
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  ret

public tss_flush
tss_flush:
  mov ax, 0x2B       ; Load TSS selector (for 64-bit mode)
  ltr ax             ; Load Task Register
  ret

