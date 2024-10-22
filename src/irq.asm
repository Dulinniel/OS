[BITS 32]

global programmable_interrupt_timer
global keyboard_interrupt
global cascade
global com2 ; Printer
global com1 ; Printer
global lpt2 ; Printer
global floppy_disk
global lpt1 ; Unreliable printer
global CMOS_clock
global legacy_SCI
global SCSI
global NIC
global PS2_mouse
global FPU
global Primary_ATA_Disk
global Secondary_ATA_Disk

programmable_interrupt_timer:
  cli
  push byte 0
  push byte 32
  jmp irq_common_setup

keyboard_interrupt:
  cli
  push byte 0
  push byte 33
  jmp irq_common_setup

cascade:
  cli
  push byte 0
  push byte 34
  jmp irq_common_setup

com2:
  cli
  push byte 0
  push byte 35
  jmp irq_common_setup

com1:
  cli
  push byte 0
  push byte 36
  jmp irq_common_setup

lpt2:
  cli
  push byte 0
  push byte 37
  jmp irq_common_setup

floppy_disk:
  cli
  push byte 0
  push byte 38
  jmp irq_common_setup

lpt1:
  cli
  push byte 0
  push byte 39
  jmp irq_common_setup

CMOS_clock:
  cli
  push byte 0
  push byte 40
  jmp irq_common_setup

legacy_SCI:
  cli
  push byte 0
  push byte 41
  jmp irq_common_setup

SCSI:
  cli
  push byte 0
  push byte 42
  jmp irq_common_setup

NIC:
  cli
  push byte 0
  push byte 43
  jmp irq_common_setup

PS2_mouse:
  cli
  push byte 0
  push byte 44
  jmp irq_common_setup

FPU:
  cli
  push byte 0
  push byte 45
  jmp irq_common_setup

Primary_ATA_Disk:
  cli
  push byte 0
  push byte 46
  jmp irq_common_setup

Secondary_ATA_Disk:
  cli
  push byte 0
  push byte 47
  jmp irq_common_setup

extern irq_handler

irq_common_setup:
  pusha
  push ds
  push es
  push fs
  push gs
  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov eax, esp
  push eax
  mov eax, irq_handler
  call eax
  pop gs
  pop fs
  pop es
  pop ds
  popa
  add esp, 8
  iret