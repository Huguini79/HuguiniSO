#include <so/sched.h>

#include <stdint.h>
#include <stddef.h>

#define CODE_SELECTOR 0x08
#define DATA_SELECTOR 0x10

struct task_struct tasks[64] = {0};
struct task_struct* current = &tasks[0];
struct task_struct* next = &tasks[0];

void blank()
{
    while(1) {__asm__ volatile ("sti");}
}

struct task_struct* initTask(long pid, long eip)
{
    struct task_struct* newTask = &tasks[pid];
    newTask->pid = pid;
    newTask->alarm = 0;
    newTask->signal = 0;
    newTask->state = Ready;
    uintptr_t* kernel_stack_top = 0x600000 + (pid * 4096) + 4096;
    newTask->tss.esp0 = kernel_stack_top;
    newTask->tss.ss0 = 0x10;
    newTask->tss.eip = (long)eip;
    newTask->tss.eflags = 0x202;
    uintptr_t* user_stack_top = 0x3FF00 + (pid * 8192) + 8192;
    *user_stack_top--;
    *user_stack_top = blank;
    newTask->tss.esp = user_stack_top;
    newTask->tss.eax = 0;
    newTask->tss.ecx = 0;
    newTask->tss.edx = 0;
    newTask->tss.ebx = 0;
    newTask->tss.ebp = 0;
    newTask->tss.esi = 0;
    newTask->tss.edi = 0;
    newTask->tss.es = DATA_SELECTOR;
    newTask->tss.cs = CODE_SELECTOR;
    newTask->tss.ss = DATA_SELECTOR;
    newTask->tss.ds = DATA_SELECTOR;
    newTask->tss.fs = DATA_SELECTOR;
    newTask->tss.gs = DATA_SELECTOR;
    newTask->tss.iopb = 0x80000000;
    
    setTSSDescriptor(newTask);

    return newTask;
}

void schedule()
{
    /*
        * ADVERTENCIA: Planificación de procesos muy simple.
        * Simplemente salta al siguiente de la lista, y no gestiona prioridades
        * como el algoritmo Round Robin.
        * Esta es una implementación de un Round Robin básico.
    */
    if (tasks[current->pid+1].tss.eip != 0)
    {
        next = &tasks[current->pid+1];
    
    } else
    {
        for (long n = 0; n < 64; ++n)
        {
            if (tasks[n].tss.eip != 0)
            {
                next = &tasks[n];
                break;
            }
        }
    }

    if (current->signal != 0)
    {
        psig();
    }

    current->state = Ready;
    current = next;
    current->state = Running;

    if (current->tss.eip != 0)
    {
        exec(current);
    }

}

int sys_fork()
{

}

int exec(struct task_struct* task)
{
    if (task->tss.eip != 0)
    {
        unsigned short selector = (task->pid+4) * 8;

        volatile struct
        {
            long offset;
            unsigned short selector;

        } __attribute__((packed)) _tmp;

        _tmp.offset = 0;
        _tmp.selector = selector;

        __asm__ volatile ("ljmp %0" :: "m"(_tmp));

    }
}