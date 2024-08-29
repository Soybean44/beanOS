format ELF64

section '.text' executable
gdtr: dw 0 ; For limit storage
      dq 0 ; For base storage

public setGdt
setGdt:
  mov [gdtr], di
  mov [gdtr+2], rsi
  lgdt [gdtr]
  ret
public reloadSegments
reloadSegments:
   ; Reload CS register:
   push 0x08                 ; Push code segment to stack, 0x08 is a stand-in for your code segment
   mov rax, .reload_CS ; Load address of .reload_CS into RAX
   push rax                  ; Push this value to the stack
   retfq                     ; Perform a far return, RETFQ or LRETQ depending on syntax
.reload_CS:
   ; Reload data segment registers
   mov   ax, 0x10 ; 0x10 is a stand-in for your data segment
   mov   ds, ax
   mov   es, ax
   mov   fs, ax
   mov   gs, ax
   mov   ss, ax
   ret

public tss_flush
tss_flush:
  mov ax, 0x2B
  ltr ax
  ret
