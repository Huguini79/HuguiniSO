#include <so/printk.h>

void panic(const char* str)
{
    printk(str);
    while(1) {}
}