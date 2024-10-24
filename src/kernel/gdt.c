#include <system.h>

#define GDT_TOTAL_ENTRY 3
#define CODE_SEGMENT    1
#define DATA_SEGMENT    2

// Define GDT Entry, pack to avoid compiler interferences such as optimization
struct GDT_Entry
{
  unsigned short limit_low;
  unsigned short base_low;
  unsigned char base_middle;
  unsigned char access;
  unsigned char granularity;
  unsigned char base_high;
}__attribute__((packed));

struct GDT_Ptr
{
  unsigned short limit;
  unsigned int base;
}__attribute__((packed));

struct GDT_Entry gdt_entry[GDT_TOTAL_ENTRY];
struct GDT_Ptr gdt_ptr;

extern void gdt_flush();

void print_segment_registers() 
{
  unsigned short cs, ds, ss;

  // Obtenir les valeurs des registres
  __asm__ __volatile__("mov %%cs, %0" : "=r"(cs));
  __asm__ __volatile__("mov %%ds, %0" : "=r"(ds));
  __asm__ __volatile__("mov %%ss, %0" : "=r"(ss));

  // Afficher les valeurs (en supposant que tu aies une fonction print_hex)
  print("CS: ");
  print_hex(cs);
  print("\nDS: ");
  print_hex(ds);
  print("\nSS: ");
  print_hex(ss);
  print("\n");
}

void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char granular)
{
  // Setup Descriptor table
  gdt_entry[num].base_low = ( base & 0xFFFF );
  gdt_entry[num].base_middle = ( base >> 16 ) & 0xFF;
  gdt_entry[num].base_high = ( base >> 24 ) & 0xFF;

  // Setup Descriptor limits
  gdt_entry[num].limit_low = ( limit & 0xFFFF );
  gdt_entry[num].granularity = ( limit >> 16 ) & 0x0F;

  // Setup granularitie
  gdt_entry[num].granularity |= ( granular & 0xF0 );
  gdt_entry[num].access = access;
}

void gdt_install()
{
  // Setup GDT Pointer and limit
  gdt_ptr.limit = (sizeof(struct GDT_Entry) * GDT_TOTAL_ENTRY ) - 1;
  gdt_ptr.base = &gdt_entry;

  // NULL Descriptor
  gdt_set_gate(0, 0, 0, 0, 0);

  // Set CS
  gdt_set_gate(CODE_SEGMENT, 0, 0xFFFFFFFF, 0x9A, 0xCF);

  // Set DS
  gdt_set_gate(DATA_SEGMENT, 0, 0xFFFFFFFF, 0x92, 0xCF);

  // Flush old GDT
  gdt_flush();
  print_segment_registers();

}