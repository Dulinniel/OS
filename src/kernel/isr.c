#include <system.h>

#define ACCESS_FLAG 0x08E

extern division_by_zero_handler();
extern debug_handler();
extern non_maskable_interrupt_handler();
extern breakpoint_handler();
extern overflow_handler();
extern bound_range_exceeded_handler();
extern invalid_opcode_handler();
extern device_not_available_handler();
extern double_fault_handler();
extern coprocessor_segment_overrun_handler();
extern exception_handler();
extern invalid_tss_handler();
extern segment_not_present_handler();
extern stack_segment_fault_handler();
extern general_protection_fault_handler();
extern page_fault_handler();
extern unknown_interrupt_handler();
extern floating_point_handler();
extern alignement_check_handler();
extern machine_check_handler();
extern SIMD_floating_point_handler();
extern virtualization_exception_handler();
extern control_protection_handler();
extern reserved_exception_handler_22();
extern reserved_exception_handler_23();
extern reserved_exception_handler_24();
extern reserved_exception_handler_25();
extern reserved_exception_handler_26();
extern reserved_exception_handler_27();
extern hypervisor_injection_handler();
extern VMM_communication_handler();
extern security_handler();
extern reserved_exception_handler_31();

void ISR_install()
{
  IDT_set_gate(0, (unsigned)division_by_zero_handler, 0x08, ACCESS_FLAG);
  IDT_set_gate(1, (unsigned)debug_handler, 0x08, ACCESS_FLAG);
  IDT_set_gate(2, (unsigned)non_maskable_interrupt_handler, 0x08, ACCESS_FLAG);
  IDT_set_gate(3, (unsigned)breakpoint_handler, 0x08, ACCESS_FLAG);
  IDT_set_gate(4, (unsigned)overflow_handler, 0x08, ACCESS_FLAG);
  IDT_set_gate(5, (unsigned)bound_range_exceeded_handler, 0x08, ACCESS_FLAG);
  IDT_set_gate(6, (unsigned)invalid_opcode_handler, 0x08, ACCESS_FLAG);
  IDT_set_gate(7, (unsigned)device_not_available_handler, 0x08, ACCESS_FLAG);
  IDT_set_gate(8, (unsigned)double_fault_handler, 0x08, ACCESS_FLAG);
  IDT_set_gate(9, (unsigned)coprocessor_segment_overrun_handler, 0x08, ACCESS_FLAG);
  IDT_set_gate(10, (unsigned)invalid_tss_handler, 0x08, ACCESS_FLAG);
  IDT_set_gate(11, (unsigned)segment_not_present_handler, 0x08, ACCESS_FLAG);
  IDT_set_gate(12, (unsigned)stack_segment_fault_handler, 0x08, ACCESS_FLAG);
  IDT_set_gate(13, (unsigned)general_protection_fault_handler, 0x08, ACCESS_FLAG);
  IDT_set_gate(14, (unsigned)page_fault_handler, 0x08, ACCESS_FLAG);
  IDT_set_gate(15, (unsigned)unknown_interrupt_handler, 0x08, ACCESS_FLAG);
  IDT_set_gate(16, (unsigned)floating_point_handler, 0x08, ACCESS_FLAG);
  IDT_set_gate(17, (unsigned)alignement_check_handler, 0x08, ACCESS_FLAG);
  IDT_set_gate(18, (unsigned)machine_check_handler, 0x08, ACCESS_FLAG);
  IDT_set_gate(19, (unsigned)SIMD_floating_point_handler, 0x08, ACCESS_FLAG);
  IDT_set_gate(20, (unsigned)virtualization_exception_handler, 0x08, ACCESS_FLAG);
  IDT_set_gate(21, (unsigned)control_protection_handler, 0x08, ACCESS_FLAG);
  IDT_set_gate(22, (unsigned)reserved_exception_handler_22, 0x08, ACCESS_FLAG);
  IDT_set_gate(23, (unsigned)reserved_exception_handler_23, 0x08, ACCESS_FLAG);
  IDT_set_gate(24, (unsigned)reserved_exception_handler_24, 0x08, ACCESS_FLAG);
  IDT_set_gate(25, (unsigned)reserved_exception_handler_25, 0x08, ACCESS_FLAG);
  IDT_set_gate(26, (unsigned)reserved_exception_handler_26, 0x08, ACCESS_FLAG);
  IDT_set_gate(27, (unsigned)reserved_exception_handler_27, 0x08, ACCESS_FLAG);
  IDT_set_gate(28, (unsigned)hypervisor_injection_handler, 0x08, ACCESS_FLAG);
  IDT_set_gate(29, (unsigned)VMM_communication_handler, 0x08, ACCESS_FLAG);
  IDT_set_gate(30, (unsigned)security_handler, 0x08, ACCESS_FLAG);
  IDT_set_gate(31, (unsigned)reserved_exception_handler_31, 0x08, ACCESS_FLAG);
};

unsigned char *exception_messages[] =
{
  "Division by Zero",
  "Debug",
  "Non maskable interrupt",
  "Breakpoint",
  "Overflow",
  "Bound range exceeded",
  "Invalid Opcode",
  "Device not available",
  "Double Fault",
  "Coprocessor Segment Overrun",
  "Invalid TSS",
  "Segment not present",
  "Stack-Segment fault",
  "General Protection Fault",
  "Page fault",
  "Unknown interrupt",
  "x87 Floating Point Exception",
  "Alignment Check",
  "Machine Check",
  "SIMD Floating-Point Exception",
  "Virtualization Exception",
  "Control Protection Exception",
  "Reserved",
  "Hypervisor Injection Exception",
  "VMM Comunication Exception",
  "Security Exception",
  "Reserved"
};

void fault_handler(struct Regs *registers)
{
  if (registers->int_no < 32)
  {
    // Display Exception in red
    set_text_color(0, 12);
    print((char*)exception_messages[(int)registers->int_no]);
    println(" Exception. System Halted");
    print("Error code: ");
    print_hex((char*)registers->err_code);
    for (;;);
  }
}