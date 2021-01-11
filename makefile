CFILES = $(wildcard *.c)
OFILESC = $(CFILES: .c=.o)
SFILES = $(wildcard *.s)
OFILESS = $(SFILES: .s=.o)
GCCFLAGS =-mtune=cortex-a72 -march=armv8-a -Wall -o2 -ffreestanding -nostdinc -nostdlib -nostartfiles
ASFLAGS =
 
all: clean kernel8.img

boot.o: boot.s
	as $(ASFLAGS) boot.s -o boot.o

%.o: %.c
	as $(ASFLAGS) -c $< -o $@

kernel.o: kernel.s
	as $(ASFLAGS) kernel.s -o kernel.o

ioasm.o: ioasm.s
	as $(ASFLAGS) ioasm.s -o ioasm.o

kernel8.img: kernel.o ioasm.o boot.o 
	ld -nostdlib -nostartfiles boot.o ioasm.o kernel.o -T link.ld -o kernel8.elf
	objcopy -O binary kernel8.elf kernel8.img

clean:
	rm kernel8.elf *.o *.img > /dev/null 2> /dev/null || true



