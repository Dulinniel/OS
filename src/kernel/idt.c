#include <system.h>

#define IDT_ENTRIES 256

struct IDT_Entry
{
  unsigned short base_low;        // Offset bit 0..15
  unsigned short segment_kernel;  // CS Selector in GDT or LDT
  unsigned char zero;             // This will be always 0
  unsigned char type_attributes;  // Gate type, DPL and P fields
  unsigned short base_high;       // Offset bit 16..31
}__attribute__((packed));

struct IDT_Ptr
{
  unsigned short limit;
  unsigned int base;
}__attribute__((packed));

struct IDT_Entry IDT_entry[IDT_ENTRIES];
struct IDT_Ptr IDT_ptr;

extern void division_by_zero_handler();
extern void IDT_Load();
/*
 31              15                             4               0
+---+--  --+---+-----+---+--  --+---+----+----+---+---+---+---+---+
|   Reserved   | SGX |   Reserved   | SS | PK | I | R | U | W | P |
+---+--  --+---+-----+---+--  --+---+----+----+---+---+---+---+---+
*/
void IDT_set_gate(unsigned char num, unsigned long base, unsigned short segment, unsigned char flags)
{
  IDT_entry[num].base_low = ( base & 0xFFFF );
  IDT_entry[num].segment_kernel = segment;
  IDT_entry[num].zero = 0;
  IDT_entry[num].type_attributes = flags;
  IDT_entry[num].base_high = ( base >> 16 ) & 0xFFFF;
}

void IDT_install()
{
  IDT_ptr.limit = (sizeof(struct IDT_Entry) * IDT_ENTRIES) - 1;
  IDT_ptr.base = &IDT_entry;

  // Clear the IDT and initilize it to 0
  memset(&IDT_entry, 0, sizeof(struct IDT_Entry) * IDT_ENTRIES);

  IDT_Load();
}