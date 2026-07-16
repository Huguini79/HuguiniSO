#include <so/main.h>
#include <so/pic.h>
#include <so/gdt.h>
#include <so/traps.h>
#include <so/console.h>
#include <so/printk.h>
#include <so/sched.h>

void tasking1()
{
    printk("Hello World!");
}

void kernel_init()
{
    remapPIC();
    GdtInstall();
    TrapsInstall();
    ConsoleInstall();
    printk("HuguiniSO gestionando el ordenador......\n\n# ");

    struct task_struct* task1 = initTask(0, (long)tasking1);
    exec(task1);

    while(1)__asm__ volatile ("sti");
}