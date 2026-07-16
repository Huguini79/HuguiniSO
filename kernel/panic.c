#include <so/printk.h>
#include <so/sched.h>

void panic(const char* str)
{
    printk(str);
    printk("\n\nREGISTER DUMP:\n\n");
    printk("EAX:%i, ECX:%i, EDX:%i, EBX: %i, ESI: %i, EDI: %i, ESP: %i, EIP: %i", current->tss.eax, current->tss.ecx, current->tss.edx, current->tss.ebx, current->tss.esi, current->tss.edi, current->tss.esp, current->tss.eip);
    while(1) {}
}