#include <system.h>

#define LAST_ROW              25
#define SCREEN_WIDTH          80
#define CRT_HIGH_BYTE_INDEX   15
#define CRT_LOW_BYTE_INDEX    14
#define VIDEO_MEMORY_ADDRESS  0xB8000
#define CRT_INDEX             0x3D4
#define CRT_DATA              0x3D5
#define TAB_SIZE              4

unsigned short *text_memory_ptr = (unsigned short *)VIDEO_MEMORY_ADDRESS;
int attribute = 0x0F;
int cursor_x, cursor_y = 0;

void scroll(void)
{
  // Blank character
  unsigned short blank = 0x20 | (attribute << 8);

  // Check if we pass the last line
  if (cursor_y >= LAST_ROW)
  {
    // Compute the line offset
    unsigned temp = cursor_y - LAST_ROW + 1;

    // Shift up the visible lines
    unsigned short* source_ptr = text_memory_ptr + (temp * SCREEN_WIDTH);
    unsigned short* destination_ptr = text_memory_ptr;

    // Move those lines
    memcpy(destination_ptr, source_ptr, (LAST_ROW - temp) * SCREEN_WIDTH * sizeof(unsigned short));

    // Erase last line
    unsigned short* blank_start = text_memory_ptr + (LAST_ROW - temp) * SCREEN_WIDTH;
    memsetw(blank_start, blank, SCREEN_WIDTH);

    // Move cursor
    cursor_y = LAST_ROW - 1;
  }
}

void move_cursor(void)
{
  // Equation to find index in a linear chunk of memory
  // index = (y_value * width_of_screen) + x_value;
  unsigned temp = cursor_y * SCREEN_WIDTH + cursor_x;

  // Set where the cursor is supposed to be next
  // VGA specific programming documents: http://www.brackeen.com/home/vga 
  // VGA Cursor Ports: CRT Control Register - Index is 0x3D4, Data is 0x3D5
  outportb(CRT_INDEX, CRT_LOW_BYTE_INDEX);
  outportb(CRT_DATA, temp >> 8);
  outportb(CRT_INDEX, CRT_HIGH_BYTE_INDEX);
  outportb(CRT_DATA, temp);
}

void clear()
{
  unsigned blank = 0x20 | ( attribute << 8 );

  // Set screen to plain Foreground color
  for (int i = 0; i < LAST_ROW; i++) memsetw(text_memory_ptr + i * SCREEN_WIDTH, blank, SCREEN_WIDTH);

  // Update virtual cursor and move the real one
  cursor_y = 0;
  cursor_x = 0;
  move_cursor();
}

void put_char(char character)
{
  unsigned short *where;
  unsigned color = attribute << 8;

  switch(character)
  {
  // Backspace
  case 0x08:
    if (cursor_x != 0) cursor_x--;
    break;
  // Tab
  case 0x09:
    cursor_x = ( cursor_x + TAB_SIZE ) & ~(TAB_SIZE - 1);
    break;
  // CR
  case '\r':
    cursor_x = 0;
    break;
  // CRLF
  case '\n':
    cursor_x = 0;
    cursor_y++;
    break;
  // Any other char
  default:
    where = text_memory_ptr + ( cursor_y * SCREEN_WIDTH + cursor_x );
    *where = character | color;
    cursor_x++;
    break;
  }

  if ( cursor_x >= SCREEN_WIDTH )
  {
    cursor_x = 0;
    cursor_y++;
  }

  // Scroll the screen and move the cursor
  scroll();
  move_cursor();
}

void print(char *text)
{
  for (int i = 0; i < strlen(text); i++) put_char(text[i]);
}

void println(char *text)
{
  print(text);
  put_char('\n');
}

void print_hex(unsigned int value) 
{
  char hex_chars[] = "0123456789ABCDEF";
  char buffer[9];  // 8 digits for 32-bit hex value + null terminator
  buffer[8] = '\0';
  print("0x");
  for (int i = 7; i >= 0; i--) 
  {
    // Extract 4 bits
    // Bitshift 4 bits to get the next hexadecimal number
    put_char(hex_chars[(value >> (i * 4)) & 0xF]);
  }
}

/*
BackColor:        ForeColor:
0   BLACK         8   DARK GREY
1   BLUE          9   LIGHT BLUE
2   GREEN         10  LIGHT GREEN
3   CYAN          11  LIGHT CYAN
4   RED           12  LIGHT RED
5   MAGENTA       13  LIGHT MAGENTA
6   BROWN         14  LIGHT BROWN
7   LIGHT GREY    15  WHITE
*/

void set_text_color(unsigned char forecolor, unsigned char backcolor)
{
  attribute = ( backcolor << 4 ) | ( forecolor & 0x0F );
}

void init_video(void)
{
  clear();
}