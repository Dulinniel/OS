#include <system.h>

#define INPUT_CLOCK 1193180
#define SQUARE_WAVE_MODE 0x36
#define DEFAULT_TIMER_INTERRUPT 18

int timer_tick = 0;

void timer_phase(int frequency_hz)
{
  int divisor = INPUT_CLOCK / frequency_hz; // Calculate divisor
  outportb(0x43, SQUARE_WAVE_MODE); // Set mode to wave mode
  outportb(0x40, divisor & 0xFF); // Set Low Byte divisor
  outportb(0x40, divisor >> 8); // Set High Byte divisor
}

void timer_handler(struct Regs *registers)
{
  timer_tick++;
  // Display a message every second
  if ( timer_tick % DEFAULT_TIMER_INTERRUPT == 0 ) 
  {
    set_text_color(9, 0);
    print("One second has passed\n");
  }
}

// Install timer into IRQ0
void timer_install()
{
  irq_install_handler(0, timer_handler);
}

// Simple delay function
void delay(int tick)
{
  unsigned long etick = timer_tick + tick;
  while( timer_tick < etick );
}