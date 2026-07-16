#include <so/console.h>

#include <libc/string.h>

void printk(const char* str)
{
    size_t len = strlen(str);

    for (long i = 0; i < len; ++i)
    {
        put_c(str[i]);
    }
}