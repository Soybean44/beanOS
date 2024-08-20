format ELF
public setGdt
setGdt:
  mov eax, [esp+4]
  lgdt [eax]
  ret

public reloadSegments
reloadSegments:
  jmp 0x08:.flush
.flush:
  mov eax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  ret

public tss_flush
tss_flush:
  mov ax, 0x2B
  ltr ax
  ret
