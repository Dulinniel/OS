[BITS 32]
; ISR Part
global IDT_Load
extern IDT_ptr
IDT_Load:
  lidt [IDT_ptr]
  ret

global division_by_zero_handler
global debug_handler
global non_maskable_interrupt_handler
global breakpoint_handler
global overflow_handler
global bound_range_exceeded_handler
global invalid_opcode_handler
global device_not_available_handler
global double_fault_handler
global coprocessor_segment_overrun_handler
global exception_handler
global invalid_tss_handler
global segment_not_present_handler
global stack_segment_fault_handler
global general_protection_fault_handler
global page_fault_handler
global unknown_interrupt_handler
global floating_point_handler
global alignement_check_handler
global machine_check_handler
global SIMD_floating_point_handler
global virtualization_exception_handler
global control_protection_handler
global reserved_exception_handler_22
global reserved_exception_handler_23
global reserved_exception_handler_24
global reserved_exception_handler_25
global reserved_exception_handler_26
global reserved_exception_handler_27
global hypervisor_injection_handler
global VMM_communication_handler
global security_handler
global reserved_exception_handler_31

division_by_zero_handler:
  cli
  push byte 0
  push byte 0
  jmp isr_common_stub

debug_handler:
  cli
  push byte 0
  push byte 1
  jmp isr_common_stub

non_maskable_interrupt_handler:
  cli
  push byte 0
  push byte 2
  jmp isr_common_stub

breakpoint_handler:
  cli
  push byte 0
  push byte 3
  jmp isr_common_stub

overflow_handler:
  cli
  push byte 0
  push byte 4
  jmp isr_common_stub

bound_range_exceeded_handler:
  cli
  push byte 0
  push byte 5
  jmp isr_common_stub

invalid_opcode_handler:
  cli
  push byte 0
  push byte 6
  jmp isr_common_stub

device_not_available_handler:
  cli
  push byte 0
  push byte 7
  jmp isr_common_stub

double_fault_handler:
  cli 
  push byte 8
  jmp isr_common_stub

coprocessor_segment_overrun_handler:
  cli
  push byte 0
  push byte 9
  jmp isr_common_stub

invalid_tss_handler:
  cli
  push byte 10
  jmp isr_common_stub

segment_not_present_handler:
  cli
  push byte 11
  jmp isr_common_stub

stack_segment_fault_handler:
  cli
  push byte 12
  jmp isr_common_stub

general_protection_fault_handler:
  cli
  push byte 13
  jmp isr_common_stub

page_fault_handler:
  cli
  push byte 14
  jmp isr_common_stub

unknown_interrupt_handler:
  cli
  push byte 0
  push byte 15
  jmp isr_common_stub

floating_point_handler:
  cli
  push byte 0
  push byte 16
  jmp isr_common_stub

alignement_check_handler:
  cli
  push byte 0
  push byte 17
  jmp isr_common_stub

machine_check_handler:
  cli
  push byte 0
  push byte 18
  jmp isr_common_stub

SIMD_floating_point_handler:
  cli
  push byte 0
  push byte 19
  jmp isr_common_stub

virtualization_exception_handler:
  cli
  push byte 0
  push byte 20
  jmp isr_common_stub

control_protection_handler:
  cli
  push byte 0
  push byte 21
  jmp isr_common_stub

reserved_exception_handler_22:
  cli
  push byte 0
  push byte 22
  jmp isr_common_stub

reserved_exception_handler_23:
  cli
  push byte 0
  push byte 23
  jmp isr_common_stub

reserved_exception_handler_24:
  cli
  push byte 0
  push byte 24
  jmp isr_common_stub

reserved_exception_handler_25:
  cli 
  push byte 0
  push byte 25
  jmp isr_common_stub

reserved_exception_handler_26:
  cli
  push byte 0
  push byte 26
  jmp isr_common_stub

reserved_exception_handler_27:
  cli
  push byte 0
  push byte 27
  jmp isr_common_stub

hypervisor_injection_handler:
  cli
  push byte 0
  push byte 28
  jmp isr_common_stub

VMM_communication_handler:
  cli
  push byte 0
  push byte 29
  jmp isr_common_stub

security_handler:
  cli
  push byte 0
  push byte 30
  jmp isr_common_stub

reserved_exception_handler_31:
  cli
  push byte 0
  push byte 31
  jmp isr_common_stub

extern fault_handler

isr_common_stub:
  pusha
  push ds
  push es
  push fs
  push gs
  mov ax, 0x10 ; Load DS descriptor 
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov eax, esp ; Push the stack
  push eax
  mov eax, fault_handler
  call eax     ; Preserve eip register
  pop eax
  pop gs
  pop fs
  pop es
  pop ds
  popa
  add esp, 8  ; Clean push error code
  iret        ; Pop CS, EIP, EFLAGS, SS and ESP 
