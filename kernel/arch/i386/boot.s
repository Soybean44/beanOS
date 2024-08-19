format ELF 
; Declare constants for the multiboot header.
MALIGN   =  1 shl 0 
MINFO    =  1 shl 1 
MBFLAGS  =  MALIGN or MINFO          ; this is the Multiboot 'flag' field
MAGIC    =  0x1BADB002          ; 'magic number' lets bootloader find the header
CHECKSUM = -(MAGIC + MBFLAGS)   ; checksum of above, to prove we are multiboot

; Declare a multiboot header that marks the program as a kernel.
section '.multiboot.data' align 4
dd MAGIC
dd MBFLAGS
dd CHECKSUM

; Allocate the initial stack.
section '.bootstrap_stack' writeable align 4 
stack_bottom:
rb 16384                            ; 16 KiB
stack_top:

; Preallocate pages used for paging.
section '.bss' writeable align 4096 
boot_page_directory:
  rb 4096
boot_page_table1:
  rb 4096

; The kernel entry point.
section '.multiboot.text' executable
public _start
_start:
  ; Physical address of boot_page_table1.
  mov edi, boot_page_table1 - 0xC0000000

  ; First address to map is address 0.
  mov esi, 0

  ; Map 1023 pages.
  mov ecx, 1023

  extrn _kernel_start
  extrn _kernel_end
.map_pages:
  ; Only map the kernel.
  cmp esi, _kernel_start
  jl .map_next
  cmp esi, _kernel_end - 0xC0000000
  jge .done

  ; Map physical address as "present, writable".
  mov edx, esi
  or edx, 0x003
  mov [edi], edx

.map_next:
  ; Size of page is 4096 bytes.
  add esi, 4096
  ; Size of entries in boot_page_table1 is 4 bytes.
  add edi, 4
  ; Loop to the next entry if we haven't finished.
  loop .map_pages

.done:
  ; Map VGA video memory to 0xC03FF000 as "present, writable".
  mov dword [boot_page_table1 - 0xC0000000 + 1023 * 4], 0x000B8000 or 0x003

  ; Map the page table to both virtual addresses 0x00000000 and 0xC0000000.
  mov dword [boot_page_directory - 0xC0000000], boot_page_table1 - 0xC0000000 + 0x003
  mov dword [boot_page_directory - 0xC0000000 + 768 * 4], boot_page_table1 - 0xC0000000 + 0x003

  ; Set cr3 to the address of the boot_page_directory.
  mov ecx, boot_page_directory - 0xC0000000
  mov cr3, ecx

  ; Enable paging and the write-protect bit.
  mov ecx, cr0
  or ecx, 0x80010000
  mov cr0, ecx

  ; Jump to higher half with an absolute jump.
  lea ecx, [_start_higher_half]
  jmp ecx

section '.text' executable
  _start_higher_half:
  ; Paging is now fully set up and enabled.

  ; Unmap the identity mapping.
  mov dword [boot_page_directory], 0

  ; Reload cr3 to force a TLB flush.
  mov ecx, cr3
  mov cr3, ecx

  ; Set up the stack.
  mov esp, stack_top

  ; Enter the high-level kernel.
  extrn kernel_main
  call kernel_main

  ; Infinite loop if the system has nothing more to do.
  cli
.halt_loop:
  hlt
  jmp .halt_loop
