#include <so/main.h>
#include <so/pic.h>
#include <so/gdt.h>
#include <so/traps.h>
#include <so/console.h>
#include <so/printk.h>
#include <so/sched.h>
#include <so/signal.h>

void tasking1()
{
        printk("\nTask1!");

    while(1)
    {
        put_cxy('A', 79, 0);
        put_cxy('1', 79, 0);
        __asm__ volatile ("sti");
    }
}

void tasking2()
{
    printk("\nTask2!");
}

void tasking3()
{
    printk("\nTask3!");
}

void kernel_init()
{
    remapPIC();
    GdtInstall();
    TrapsInstall();
    ConsoleInstall();
    printk("HuguiniSO gestionando el ordenador......\n\n# ");

    struct task_struct* task1 = initTask(0, (long)tasking1);
    struct task_struct* task2 = initTask(1, (long)tasking2);
    struct task_struct* task3 = initTask(2, (long)tasking3);

    while(1)__asm__ volatile ("sti");
}