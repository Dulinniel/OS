#include <system.h>

// copy count bytes of data from src to dest, return 'destination'
unsigned short *memcpy(unsigned short *destination, const unsigned short *source, int count)
{
  const unsigned short *source_pointer = (const short)source;
  short *desination_pointer = (short *)destination;
  for (int i = count; i != 0; i--) *desination_pointer++ = *source_pointer++;
  return destination;
}

// Set count bytes in dest to val, return 'destination'
unsigned char *memset(unsigned char *destination, unsigned char value, int count)
{
  unsigned char *temp = (char*)destination;
  for (int i = count; i != 0; i--) *temp++ = value;
  return destination;
}

// Memset but with 16 bits
unsigned short *memsetw(unsigned short *destination, unsigned short value, int count)
{
  unsigned short *temp = (unsigned short *)destination;
  for (int i = count; i != 0; i--) *temp++ = value;
  return destination;
}

// Return the length of a string in bytes
int strlen(const char *str)
{
  int val;
  for (val = 0; *str != '\0'; str++) val++;
  return val;
}

// Reading from the I/O ports to get data
unsigned char inportb(unsigned short _port)
{
  unsigned char rv;
  __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
  return rv;
}

// Writing to I/O ports to send byte to devices
void outportb(unsigned short _port, unsigned char _data)
{
  __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a"  (_data));
}

void main()
{
  init_video();
  gdt_install();
  IDT_install();
  ISR_install();
  irq_install();
  //timer_install();
  set_text_color(9, 0);
  for (int i = 0; i < 300; i++)
  {
    print_hex(i);
  }
  clear();
  print('\n');
  print('\n');  
  set_text_color(13, 0);
  println("Welcome to this OS\n");
  set_text_color(15, 0);
  println("Hello world!\n");
  __asm__ __volatile__("sti");
  for(;;);
}
