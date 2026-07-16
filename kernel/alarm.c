#include <so/alarm.h>

struct callout callouts[64] = {0};

void alarm(long s)
{
    long ms = s * 1000;
    callouts[current->pid].ticks = ms / 10;
    callouts[current->pid].task = current;
}