#ifndef ALARM_H
#define ALARM_H

#include <so/sched.h>

struct callout
{
    long ticks;
    struct task_struct* task;
};

extern struct callout callouts[64];

void alarm(long s);

#endif