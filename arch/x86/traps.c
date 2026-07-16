#include <so/ioport.h>
#include <so/panic.h>

struct Idt
{
    unsigned short isr_low;
    unsigned short selector;
    unsigned char reserved;
    unsigned char type_attributes;
    unsigned short isr_high;

} __attribute__((packed));

struct Idtr
{
    unsigned short limit;
    long base;

} __attribute__((packed));

struct Idtr idtr;

struct Idt idt_table[256] __attribute__((aligned(8)));

void setIdtDescriptor(long vector, unsigned char type_attributes, void* isr)
{
    struct Idt* Idt = &idt_table[vector];
    Idt->isr_low = (long)isr & 0xFFFF;
    Idt->selector = 0x08;
    Idt->reserved = 0;
    Idt->type_attributes = type_attributes;
    Idt->isr_high = (long)isr >> 16 & 0xFFFF;
}

void divide_by_zero()
{
    panic("* KERNEL PANIC - Divide by zero error *");
}

volatile long ticks = 0;

void clock_handler()
{
    ticks += 6;
    outb(0x20, 0x20);
}

void keyboard_handler()
{
    unsigned char scancode = insb(0x60);
    const char scancode_to_ascii[128] = {
        0,   27,  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', // 0x00 - 0x0E
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',    // 0x0F - 0x1C
        0,  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',          // 0x1D - 0x29
        0,  '\\','z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',   0,           // 0x2A - 0x36
    '*',   0, ' '                                                              // 0x37 - 0x39
    };

    if (scancode & 0x80)
    {

    } else
    {
        put_c(scancode_to_ascii[scancode]);
    }

    outb(0x20, 0x20);
}

void TrapsInstall()
{
    setIdtDescriptor(0, 0x8E, divide_by_zero);
    setIdtDescriptor(0x20, 0x8E, clock_handler);
    setIdtDescriptor(0x21, 0x8E, keyboard_handler);

    idtr.limit = sizeof(idt_table) - 1;
    idtr.base = (long)idt_table;

    __asm__ volatile ("lidt %0" :: "m"(idtr));
}