
clean:
	rm *.o kerneldraft
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


