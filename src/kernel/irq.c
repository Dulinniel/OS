#include <system.h>

#define IRQ_LAST_ENTRY 40
#define IRQ_HANDLER_AMOUNT 16
#define REMAP_OFFSET 32
#define EOI 0x20

extern void programmable_interrupt_timer();
extern void keyboard_interrupt();
extern void cascade();
extern void com2();
extern void com1();
extern void lpt2();
extern void floppy_disk();
extern void lpt1();
extern void CMOS_clock();
extern void legacy_SCI();
extern void SCSI();
extern void NIC();
extern void PS2_mouse();
extern void FPU();
extern void Primary_ATA_Disk();
extern void Secondary_ATA_Disk();

void *irq_routines[IRQ_HANDLER_AMOUNT] = 
{
  0, 0, 0, 0, 
  0, 0, 0, 0,
  0, 0, 0, 0, 
  0, 0, 0, 0
};

void irq_install_handler(int irq, void (*handler)(struct Regs *registers))
{
  irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq, void (*handler)(struct Regs *registers))
{
  irq_routines[irq] = 0;
}

void irq_remap(void)
{
  //send commands to the PIC in order to make IRQ0 to 15 be remapped to IDT entries 32 to 47
  outportb(0x20, 0x11);
  outportb(0xA0, 0x11);
  outportb(0x21, 0x20);
  outportb(0xA1, 0x11);
  outportb(0x21, 0x04);
  outportb(0xA1, 0x02);
  outportb(0x21, 0x01);
  outportb(0xA1, 0x01);
  outportb(0x21, 0x0);
  outportb(0xA1, 0x0);
}

void irq_install()
{
  // Remap the interrupt controllers
  irq_remap();
  // Install the appropriate ISRs to the correct entries in the IDT
  IDT_set_gate(32, (unsigned)programmable_interrupt_timer, 0x08, 0x8E);
  IDT_set_gate(33, (unsigned)keyboard_interrupt, 0x08, 0x8E);
  IDT_set_gate(34, (unsigned)cascade, 0x08, 0x8E);
  IDT_set_gate(35, (unsigned)com2, 0x08, 0x8E);
  IDT_set_gate(36, (unsigned)com1, 0x08, 0x8E);
  IDT_set_gate(37, (unsigned)lpt2, 0x08, 0x8E);
  IDT_set_gate(38, (unsigned)floppy_disk, 0x08, 0x8E);
  IDT_set_gate(39, (unsigned)lpt1, 0x08, 0x8E);
  IDT_set_gate(40, (unsigned)CMOS_clock, 0x08, 0x8E);
  IDT_set_gate(41, (unsigned)legacy_SCI, 0x08, 0x8E);
  IDT_set_gate(42, (unsigned)SCSI, 0x08, 0x8E);
  IDT_set_gate(43, (unsigned)NIC, 0x08, 0x8E);
  IDT_set_gate(44, (unsigned)PS2_mouse, 0x08, 0x8E);
  IDT_set_gate(45, (unsigned)FPU, 0x08, 0x8E);
  IDT_set_gate(46, (unsigned)Primary_ATA_Disk, 0x08, 0x8E);
  IDT_set_gate(47, (unsigned)Secondary_ATA_Disk, 0x08, 0x8E);
}

void irq_handler(struct Regs *registers)
{
  void (*handler)(struct Regs *registers);

  // Check if a custom handler exist for the selected IRQ
  handler = irq_routines[registers->int_no - REMAP_OFFSET];
  if ( handler ) handler(registers);
  // If the IDT entry is out of range, send EOI
  if ( registers->int_no >= IRQ_LAST_ENTRY ) outportb(0x0A, EOI);

  // Send EOI to interrupt controller
  outportb(EOI, EOI);
}