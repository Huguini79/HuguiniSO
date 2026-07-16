#ifndef LIBC_STRING_H
#define LIBC_STRING_H

#include <stdint.h>
#include <stddef.h>

static inline size_t strlen(const char* str)
{
    size_t i = 0;

    while(str[i] != '\0')
    {
        i++;
    }

    return i;
}

#endif