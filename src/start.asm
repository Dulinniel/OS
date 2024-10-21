; Kernel entry point
; Interrupts disabled for now

[BITS 32]
global start
start:
  mov esp, _sys_stack ; Point stack to new stack area
  jmp stublet

ALIGN 4 ; Align the next part to 4 byte
mboot:
  ; Just some boot macro
  MULTIBOOT_PAGE_ALIGN equ 1<<0
  MULTIBOOT_MEMORY_INFO equ 1<<1
  MULTIBOOT_AOUT_KLUDGE equ 1<<16
  MULTIBOOT_HEADER_MAGIC equ 0x1BADB002
  MULTIBOOT_HEADER_FLAGS equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_AOUT_KLUDGE
  MULTIBOOT_CHECKSUM equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS) ; Should be equal to 0
  EXTERN code, bss, end

  ; Boot signature for GRUB
  dd MULTIBOOT_HEADER_MAGIC
  dd MULTIBOOT_HEADER_FLAGS
  dd MULTIBOOT_CHECKSUM

  ; AOUT kludge - must be physical addresses
  ; The linker will fill what's below

  dd mboot
  dd code
  dd bss
  dd end
  dd start

; Infinite loop
; entry point for extern_main and call_main

stublet:
  extern main
  call main
  jmp $

; ISR Will be here





; Define BSS section, storing the stack
SECTION .bss
  resb 8192 ; 8kB of memory reserved 
_sys_stack: