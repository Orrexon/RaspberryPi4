.section ".text.boot"

.global _start

_start:
//check processor ID == 0 to execute on main core

	mrs x1, mpidr_el1
	and x1, x1, #3
	cbz x1, TWO; #if it is zero, jump to lbl 2 and clean bss

ONE:	
	#else, wait for event
	wfe
	b ONE

TWO:
//set stack to start below code
	ldr x1, =_start
	mov sp, x1

//enable some things in the system control register
	mov x2, #0x0800
	movk x2, #0x30D0, lsl #16
	msr sctlr_el1, x2

	#clean the bss
	ldr x1, =__bss_start
	ldr x2, =__bss_size
THREE:	cbz x2, FOUR
	str xzr, [x1], #8

	sub x2, x2, #1
	 #keep going until r2 is zero
	cbnz x2,THREE 

FOUR:	
	#jump to program
//	bl main_asm
	bl main
	b ONE
