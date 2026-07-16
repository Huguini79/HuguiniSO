#include <so/ioport.h>
#include <so/panic.h>
#include <so/signal.h>
#include <so/alarm.h>

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

volatile long ticks = 0;

void clock_handler()
{
    outb(0x20, 0x20);
    ticks += 6;

    for (long i = 0; i < 64; ++i)
    {
        if (callouts[i].ticks != 0)
        {
            if (callouts[i].ticks <= ticks)
            {
                sendsig(callouts[i].task, SIGALRM);
                callouts[i].ticks = 0;
            }
        }
    }

    schedule();
}

char keyboard_buffer[1024];
long pos = 0;

void addCharacter(char c)
{
    keyboard_buffer[pos++] = c;
    keyboard_buffer[pos] = '\0';
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
        if (scancode_to_ascii[scancode] != '\n')
        {
            put_c(scancode_to_ascii[scancode]);
            addCharacter(scancode_to_ascii[scancode]);

        } else
        {
            printk("\n# ");
        }
    }

    outb(0x20, 0x20);
}

void divide_by_zero_error()
{
	panic("Divide by zero error\n");
}

void debug()
{
	panic("* KERNEL PANIC - DEBUG *");
}

void nmi()
{
	panic("* KERNEL PANIC - NMI *");
}

void int3()
{
	panic("* KERNEL PANIC - INT3 *");
}

void overflow()
{
	panic("* KERNEL PANIC - OVERFLOW *");
}

void bounds()
{
	panic("* KERNEL PANIC - BOUNDS *");
}

void invalid_op()
{
	panic("* KERNEL PANIC - INVALID OP *");
}

void device_not_available()
{
	panic("* KERNEL PANIC - DEVICE NOT AVAILABLE *");
}

void double_fault()
{
	panic("* KERNEL PANIC - Double Fault #DF *");
}

void coprocessor_segment_overrun()
{
	panic("* KERNEL PANIC - Coprocessor Segment Overrun *");
}

void invalid_tss()
{
	panic("* KERNEL PANIC - Invalid TSS *");
}

void segment_not_present()
{
	panic("* KERNEL PANIC - Segment not present *");
}

void stack_segment()
{
	panic("* KERNEL PANIC - Stack Segment *");
}

void general_protection()
{
	panic("* KERNEL PANIC - General Protection Fault #GP *");
}

void page_fault()
{
	panic("* KERNEL PANIC - Page Fault #PF *");
}

void reserved()
{
	panic("* KERNEL PANIC - Reserved *");
}

void coprocessor_error()
{
	panic("* KERNEL PANIC - Coprocessor error *");
}

void TrapsInstall()
{
    setIdtDescriptor(0, 0x8E, divide_by_zero_error);
    setIdtDescriptor(0x01, 0x8E, debug);
	setIdtDescriptor(0x02, 0x8E, nmi);
	setIdtDescriptor(0x03, 0x8E, int3);
	setIdtDescriptor(0x04, 0x8E, overflow);
	setIdtDescriptor(0x05, 0x8E, bounds);
	setIdtDescriptor(0x06, 0x8E, invalid_op);
	setIdtDescriptor(0x07, 0x8E, device_not_available);
	setIdtDescriptor(0x08, 0x8E, double_fault);
	setIdtDescriptor(0x09, 0x8E, coprocessor_segment_overrun);
	setIdtDescriptor(0x10, 0x8E, invalid_tss);
	setIdtDescriptor(0x11, 0x8E, segment_not_present);
	setIdtDescriptor(0x12, 0x8E, stack_segment);
	setIdtDescriptor(0x13, 0x8E, general_protection);
	setIdtDescriptor(0x14, 0x8E, page_fault);
	setIdtDescriptor(0x15, 0x8E, reserved);
	setIdtDescriptor(0x16, 0x8E, coprocessor_error);
    setIdtDescriptor(0x20, 0x8E, clock_handler);
    setIdtDescriptor(0x21, 0x8E, keyboard_handler);

    idtr.limit = sizeof(idt_table) - 1;
    idtr.base = (long)idt_table;

    __asm__ volatile ("lidt %0" :: "m"(idtr));
}