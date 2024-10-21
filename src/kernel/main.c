#include <system.h>

// copy count bytes of data from src to dest, return 'dest'
unsigned char *memcpy(unsigned char *dest, const unsigned char *src, int count)
{
  const char *sp = (const char)src;
  char *dp = (char *)dest;
  for (int i = count; i != 0; i--) *dp++ = *sp++;
  return dest;
}

// Set count bytes in dest to val, return 'dest'
unsigned char *memset(unsigned char *dest, unsigned char val, int count)
{
  char *temp = (char*)dest;
  for (int i = count; i != 0; i--) *temp++ = val;
  return dest;
}

// Memset but with 16 bits
unsigned short *memsetw(unsigned short *dest, unsigned short val, int count)
{
  unsigned short *temp = (unsigned short *)dest;
  for (int i = count; i != 0; i--) *temp++ = val;
  return dest;
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
  print("Hello");
  for (;;);
}