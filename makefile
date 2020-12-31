CFILES = $(wildcard *.c)
OFILESC = $(CFILES: .c=.o)
SFILES = $(wildcard *.s)
OFILESS = $(SFILES: .s=.o)
GCCFLAGS =-mtune=cortex-a72 -march=armv8-a -Wall -o2 -ffreestanding -nostdinc -nostdlib -nostartfiles
ASFLAGS = 
GCCPATH = /opt/aarch64/bin/
GCC = $(GCCPATH)aarch64-linux-gnu-gcc 
all: clean kernel8.img

boot.o: boot.s
	as $(ASFLAGS) -c boot.s -o boot.o

%.o: %.c
	as $(ASFLAGS) -c $< -o $@

%.o: %.s
	as $(ASFLAGS) -c $< -o $@

kernel8.img: boot.o $(OFILES) $(OFILESS)
	ld -nostdlib -nostartfiles boot.o $(OFILES) $(OFILESS) -T link-ld -o kernel.elf
	objcopy -O binary kernel8.elf kernel8.img

clean:
	rm kernel8.elf *.o *.img > /dev/null 2> /dev/null || true



