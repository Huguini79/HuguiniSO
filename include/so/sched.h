#ifndef SCHED_H
#define SCHED_H

struct tss
{
    long prev_link;
    long esp0;
    long ss0;
    long esp1;
    long ss1;
    long esp2;
    long ss2;
    long cr3;
    long eip;
    long eflags;
    long eax;
    long ecx;
    long edx;
    long ebx;
    long esp;
    long ebp;
    long esi;
    long edi;
    long es;
    long cs;
    long ss;
    long ds;
    long fs;
    long gs;
    long ldtr;
    long iopb;

} __attribute__((packed));

enum State
{
    Ready,
    Running,
    Blocked,
    Zombie,
    Sleep,
    Swapper,
};

struct task_struct
{
    long pid; /* Identificador de proceso */
    long ppid; /* Proceso padre */
    long alarm; /* Alarma */
    long signal; /* Señales */
    enum State state;
    struct tss tss; /* Contexto del proceso */
};

extern struct task_struct tasks[64];
extern struct task_struct* current;

struct task_struct* initTask(long pid, long eip);
void schedule();
int sys_fork();
int exec(struct task_struct* task);

#endif