#include <so/ioport.h>

void remapPIC()
{
    /* ICW 1 */
    unsigned char icw1_packet = 0x11;
    outb(0x20, icw1_packet);
    outb(0xA0, icw1_packet);

    /* ICW 2 */
    unsigned char icw2_packet_master = 0x20;
    unsigned char icw2_packet_slave = 0x28;
    outb(0x21, icw2_packet_master);
    outb(0xA1, icw2_packet_slave);

    /* ICW 3 */
    unsigned char icw3_packet_master = 0x4;
    unsigned char icw3_packet_slave = 0x2;
    outb(0x21, icw3_packet_master);
    outb(0xA1, icw3_packet_slave);

    /* ICW 4 */
    unsigned char icw4_packet = 1;
    outb(0x21, icw4_packet);
    outb(0xA1, icw4_packet);
}