CFILES = $(wildcard *.c)
OFILESC = $(CFILES: .c=.o)
SFILES = $(wildcard *.s)
OFILESS = $(SFILES: .s=.o)
GCCFLAGS = -Wall -o2 -ffreestanding -nostdinc -nostdlib -nostartfiles
ASFLAGS =
 
all: clean kernel8.img

boot.o: boot.s
	as $(ASFLAGS) boot.s -o boot.o

%.o: %.c
	as $(ASFLAGS) -c $< -o $@

kernel.o: kernel.s
	as $(ASFLAGS) kernel.s -o kernel.o

kernelc.o: kernelc.c
	gcc $(GCCFLAGS) -c kernelc.c -o kernelc.o

ioc.o: ioc.c
	gcc $(GCCFLAGS) -c ioc.c -o ioc.o

kernel8.img: kernelc.o ioc.o boot.o 
	ld -nostdlib -nostartfiles boot.o ioc.o kernelc.o -T link.ld -o kernel8.elf
	objcopy -O binary kernel8.elf kernel8.img

clean:
	rm kernel8.elf *.o *.img > /dev/null 2> /dev/null || true



