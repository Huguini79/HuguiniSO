#include <so/console.h>

#include <libc/string.h>

#include <stdarg.h>

void printk(const char* fmt, ...)
{
    va_list args;
    char buf[16];
    size_t len = strlen(fmt);

    va_start(args, fmt);

    for (long i = 0; i < len; ++i)
    {
        if (fmt[i] == '%')
        {
            i++;
            if (fmt[i] == 'i')
            {
                int num = va_arg(args, int);
                itoa(num, buf, 10);
                printk(buf);
            }
        } else
        {
            put_c(fmt[i]);
        }
    }
}