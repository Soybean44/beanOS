format ELF64

section '.text' executable

public idt_flush
idt_flush:
  mov rax, [rsp+8]   ; Use RSP instead of ESP and RAX instead of EAX
  lidt [rax]         ; Load the IDT
  sti                ; Set Interrupt Flag
  ret

; Macros for defining ISRs and IRQs

macro ISR_NOERRCODE num {
  public isr#num
  isr#num:
    cli
    xor rax, rax       ; Clear RAX, which is 64-bit
    push rax           ; Push error code (0)
    mov rax, num
    push rax           ; Push interrupt number
    jmp isr_common_stub
}

macro ISR_ERRCODE num {
  public isr#num
  isr#num:
    cli
    mov rax, num
    push rax           ; Push interrupt number
    jmp isr_common_stub
}

macro IRQ num, arg {
  public irq#num
  irq#num:
    cli
    xor rax, rax       ; Clear RAX, which is 64-bit
    push rax           ; Push error code (0)
    mov rax, arg
    push rax           ; Push interrupt number
    jmp irq_common_stub
}

; Define ISRs (Interrupt Service Routines) without error codes

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7

ISR_ERRCODE 8
ISR_NOERRCODE 9
ISR_ERRCODE 10
ISR_ERRCODE 11
ISR_ERRCODE 12
ISR_ERRCODE 13
ISR_ERRCODE 14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31
ISR_NOERRCODE 128
ISR_NOERRCODE 177

; Define IRQs (Interrupt Requests)

IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47

; Common ISR and IRQ stubs

extrn isr_handler
isr_common_stub:
  push rax
  push rcx
  push rdx
  push rbx
  push rsp
  push rbp
  push rsi
  push rdi
  push r8
  push r9
  push r10
  push r11
  push r12
  push r13
  push r14
  push r15

  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  push rsp
  cld
  call isr_handler

  add rsp, 8
  pop r15
  pop r14
  pop r13
  pop r12
  pop r11
  pop r10
  pop r9
  pop r8
  pop rdi
  pop rsi
  pop rbp
  pop rsp
  pop rbx
  pop rdx
  pop rcx
  pop rax

  sti
  iretq

extrn irq_handler
irq_common_stub:
  push rax
  push rcx
  push rdx
  push rbx
  push rsp
  push rbp
  push rsi
  push rdi
  push r8
  push r9
  push r10
  push r11
  push r12
  push r13

