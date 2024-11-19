#include <system.h>

// copy count bytes of data from src to dest, return 'destination'
unsigned short *memcpy(unsigned short *destination, const unsigned short *source, int count)
{
  const unsigned short *source_pointer = source;
  unsigned short *desination_pointer = destination;
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

void *memmove(void *destination, const void *source, char size)
{
  char *dest = destination;
  const char *src = source;
  char *last_src = src + (size - 1);
  char *last_dest = dest + (size - 1);
  if ( dest < src ) while (size--) *dest++ = *src++;
  else while (size--) *last_dest-- = *last_src--;
  
  return dest;
}

// Return the length of a string in bytes
int strlen(const char *str)
{
  int length;
  for (length = 0; *str != '\0'; str++) length++;
  return length;
}

// Reading from the I/O ports to get data
unsigned char inportb(unsigned short _port)
{
  unsigned char read_value;
  __asm__ __volatile__ ("inb %1, %0" : "=a" (read_value) : "dN" (_port));
  return read_value;
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
  for (int i = 0; i < 601; i++)
  {
    print_hex(i);
    print("\n");
  }
  print('\n');
  print('\n');  
  set_text_color(13, 0);
  println("Welcome to this OS\n");
  set_text_color(15, 0);
  println("Hello world!\n");
  __asm__ __volatile__("sti");
  for(;;);
}
