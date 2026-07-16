struct Gdt
{
    unsigned short limit;
    unsigned short base_low;
    unsigned char base_mid;
    unsigned char access_byte;
    unsigned char flags;
    unsigned char base_high;

} __attribute__((packed));

struct Gdtr
{
    unsigned short limit;
    long base;

} __attribute__((packed));

struct Gdtr gdtr;

struct Gdt gdt_table[8192] __attribute__((aligned(8)));

void setGdtDescriptor(long n, unsigned short limit, long base, unsigned char access_byte, unsigned char flags)
{
    struct Gdt* Gdt = &gdt_table[n];
    Gdt->limit = limit;
    Gdt->base_low = (long)base & 0xFFFF;
    Gdt->base_mid = (long)base >> 16 & 0xFF;
    Gdt->access_byte = access_byte;
    Gdt->flags = flags;
    Gdt->base_high = (long)base >> 24 & 0xFF;
}

void GdtInstall()
{
    setGdtDescriptor(0, 0, 0, 0, 0);
    setGdtDescriptor(1, 0xFFFF, 0, 0x9B, 0xCF);
    setGdtDescriptor(2, 0xFFFF, 0, 0x93, 0xCF);

    gdtr.limit = sizeof(gdt_table) - 1;
    gdtr.base = (long)gdt_table;

    __asm__ volatile ("lgdt %0" :: "m"(gdtr));
}