#ifndef __SYSTEM_H
#define __SYSTEM_H

// MAIN.C
extern unsigned char *memcpy(unsigned char *dest, const unsigned char *src, int count);
extern unsigned char *memset(unsigned char *dest, unsigned char val, int count);
extern unsigned short *memsetw(unsigned short *dest, unsigned short val, int count);
extern int strlen(const char *str);
extern unsigned char inportb(unsigned short _port);
extern void outportb(unsigned short _port, unsigned char _data);

// SCREEN.H
extern void clear();
extern void put_char(unsigned char character);
extern void print(unsigned char *str);
extern void set_text_color(unsigned char forecolor, unsigned char backcolor);
extern void init_video();

#endif