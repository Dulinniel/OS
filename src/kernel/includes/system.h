#ifndef __SYSTEM_H
#define __SYSTEM_H

// MAIN.C
extern unsigned short *memcpy(unsigned short *destination, const unsigned short *source, int count);
extern unsigned char *memset(unsigned char *destination, unsigned char value, int count);
extern unsigned short *memsetw(unsigned short *destination, unsigned short value, int count);
extern int strlen(const char *str);
extern unsigned char inportb(unsigned short _port);
extern void outportb(unsigned short _port, unsigned char _data);

// SCREEN.C
extern void clear();
extern void put_char(char character);
extern void print(char *text);
void println(char *text);
extern void print_hex(unsigned int value);
extern void set_text_color(unsigned char forecolor, unsigned char backcolor);
extern void init_video();

// GDT.C
extern void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char granular);
extern void gdt_install();

// IDT.C
extern void IDT_set_gate(unsigned char num, unsigned long base, unsigned short segment, unsigned char flags);
extern void IDT_install();
extern void check_idt();

// ISR.C
struct Regs
{
  unsigned int gs, fs, es, ds; // pushed the last segments
  unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax; // pushed by pusha
  unsigned int int_no, err_code; // push byte #
  unsigned int eip, cs, eflags, useresp, ss; // pushed by the processor 
};
extern void ISR_install();

// IRQ.C
void irq_install_handler(int irq, void (*handler)(struct Regs *registers));
void irq_uninstall_handler(int irq, void (*handler)(struct Regs *registers));
void irq_install();

// PIT.C
void timer_install();
void delay(int tick);
void timer_phase(int frequency_hz);

#endif