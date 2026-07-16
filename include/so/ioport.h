#ifndef IOPORT_H
#define IOPORT_H

unsigned char insb(unsigned short port);
unsigned short insw(unsigned short port);
long inl(unsigned short port);

void outb(unsigned short port, unsigned char data);
void outw(unsigned short port, unsigned short data);
void outl(unsigned short port, long data);

#endif