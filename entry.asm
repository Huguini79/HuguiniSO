BITS 32

section .multiboot
align 4
dd 0x1BADB002
dd 0x00
dd - (0x1BADB002 + 0x00)

section .bss
stack_bottom:
    times 65536 db 0

stack_top:

section .text
global start
extern kernel_init

start:
    cli
    call kernel_init
    hlt