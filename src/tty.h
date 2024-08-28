#ifndef TTY_H
#define TTY_H
void drawchar_transparent(unsigned char c, int x, int y, int fgcolor);
void tty_newline();
void putchar(char c);
void write(char* msg, unsigned int size);
void write_string(char* msg);
#endif

