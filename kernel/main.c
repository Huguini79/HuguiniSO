#include <so/main.h>
#include <so/pic.h>
#include <so/gdt.h>
#include <so/traps.h>
#include <so/console.h>
#include <so/printk.h>

void kernel_init()
{
    remapPIC();
    GdtInstall();
    TrapsInstall();
    ConsoleInstall();
    printk("HuguiniSO gestionando el ordenador......");
    while(1)__asm__ volatile ("sti");
}