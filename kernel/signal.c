#include <so/sched.h>
#include <so/signal.h>

void signal(long signal, void* handler)
{
    current->sigactions[signal].handler = handler;
}

void sendsig(struct task_struct* task, long signal)
{
    task->signal |= 1U << (signal);
}

void psig()
{
    if (current->signal & (1U << SIGINT))
    {
        printk("* SIGINT signal for process with PID=%i", current->pid);
        current->signal &= ~(1U << SIGINT);
    }
}