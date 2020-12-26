blink: blink.o
	ld -o blink blink.o
blink.o: blink.s
	as -o blink.o blink.s

clean:
	rm *.o blink kerneldraft
kerneldraft:	
	as -g -o kerneldraft.o kerneldraft.s
	ld -o kerneldraft kerneldraft.o -T kernel.ld

debug_kerneldraft:	
	as -g -o kerneldraft.o kerneldraft.s
	ld -o kerneldraft kerneldraft.o -T kernel.ld
	gdb kerneldraft

debug_kerneldraft_no_ld:	
	as -g -o kerneldraft.o kerneldraft.s
	ld -o kerneldraft kerneldraft.o
	gdb kerneldraft

debug:	
	as -g -o blink.o blink.s
	ld -o blink blink.o
	gdb blink

