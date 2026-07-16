#include <so/console.h>

#define VGA_MEM 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define DEFAULT_FONT 0x0F

unsigned short* video_mem;
long x, y = 0;

void ConsoleInstall()
{
    video_mem = (unsigned short*)VGA_MEM;
    clear();
}

void put_cxy(char c, int x, int y)
{
    video_mem[(y * VGA_WIDTH) + x] = DEFAULT_FONT << 8 | c;
}

void put_c(char c)
{
    if (x > VGA_WIDTH)
    {
        x = 0;
        y++;
    }

    if (y >= VGA_HEIGHT)
    {
        x = 0;
        y = 0;
    }

    if (c == '\n')
    {
        x = 0;
        y++;
    
    } else
    {
        video_mem[(y * VGA_WIDTH) + x] = DEFAULT_FONT << 8 | c;
        x++;
    }
}

void clear()
{
    x = 0;
    y = 0;

    for (long i = 0; i < VGA_HEIGHT; ++i)
    {
        for (long j = 0; j < VGA_WIDTH; ++j)
        {
            video_mem[(i * VGA_WIDTH) + j] = DEFAULT_FONT << 8 | ' ';
        }
    }
}