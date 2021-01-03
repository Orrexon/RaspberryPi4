CFILES = $(wildcard *.c)
OFILESC = $(CFILES: .c=.o)
SFILES = $(wildcard *.s)
OFILESS = $(SFILES: .s=.o)
GCCFLAGS =-mtune=cortex-a72 -march=armv8-a -Wall -o2 -ffreestanding -nostdinc -nostdlib -nostartfiles
ASFLAGS = -mcpu=all -march=armv8-a
 
all: clean kernel8.img

boot.o: boot.s
	as $(ASFLAGS) boot.s -o boot.o

%.o: %.c
	as $(ASFLAGS) -c $< -o $@

kernel.o: kernel.s
	as $(ASFLAGS) kernel.s -o kernel.o

kernel8.img: boot.o kernel.o 
	ld -nostdlib -nostartfiles kernel.o boot.o -T link.ld -o kernel8.elf
	objcopy -O binary kernel8.elf kernel8.img

clean:
	rm kernel8.elf *.o *.img > /dev/null 2> /dev/null || true



