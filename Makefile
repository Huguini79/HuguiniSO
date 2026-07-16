# Detecta automáticamente todos los .c y .asm en cualquier subcarpeta
SRCS_C   = $(shell find . -name "*.c")
SRCS_ASM = $(shell find . -name "*.asm")

# Convierte las rutas de los fuentes a sus correspondientes objetos
# Ejemplo: boot/boot.asm -> boot/boot.asm.o
# Ejemplo: arch/x86/gdt.c -> arch/x86/gdt.o
OBJS_C   = $(SRCS_C:.c=.o)
OBJS_ASM = $(SRCS_ASM:.asm=.asm.o)

# Lista completa de objetos para el linker
ALL_OBJS = $(OBJS_C) $(OBJS_ASM)

# Herramientas
CC = gcc
AS = nasm
LD = ld

# Flags de compilación
CFLAGS  = -I./include -m32 -ffreestanding -nostdlib -Wall -Wextra
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T linker.ld

KERNEL = iso/boot/kernel

.PHONY: all clean

all: $(KERNEL)

# Enlazado final
$(KERNEL): $(ALL_OBJS)
	$(LD) -Map=kernel.map $(LDFLAGS) -o $@ $(ALL_OBJS)
	grub-mkrescue -o HuguiniSO.iso iso/
	qemu-system-x86_64 HuguiniSO.iso

# Regla implícita para compilar cualquier .c esté donde esté
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regla implícita para ensamblar cualquier .asm esté donde esté
%.asm.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

# Limpieza recursiva de objetos
clean:
	find . -name "*.o" -type f -delete
	rm -f $(KERNEL)