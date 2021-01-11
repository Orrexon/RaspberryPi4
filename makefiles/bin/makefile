CFILES = $(wildcard *.c)
OFILESC = $(CFILES: .c=.o)
SFILES = $(wildcard *.s)
OFILESS = $(SFILES: .s=.o)
GCCFLAGS = -Wall -o2 -ffreestanding -nostdinc -nostdlib -nostartfiles
ASFLAGS = 
 
all: clean kernel8

boot.o: boot.s
	as $(ASFLAGS) boot.s -o boot.o

%.o: %.c
	as $(ASFLAGS) -c $< -o $@

kernel.o: kernel.s
	as $(ASFLAGS) kernel.s -o kernel.o

blink.o: blink.s
	as $(ASFLAGS) blink.s -o blink.o

kernel8: boot.o blink.o kernel.o 
	ld -nostdlib -nostartfiles blink.o kernel.o boot.o -T link.ld -o kernel8

clean:
	rm kernel8.elf *.o *.img > /dev/null 2> /dev/null || true



