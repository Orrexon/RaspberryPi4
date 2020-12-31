.section ".text.boot"

.global _start

_start:
@check processor ID == 0 to execute on main core

	mrs r1, mpidr_el1
	and r1, r1, #3
	cbz r1, 2f @if it is zero, jump to lbl 2 and clean bss

1:	wfe @else, wait for event
	b 1b

2:
@set stack to start below code
	ldr r1, =_start
	mov sp, r1

@clean the bss
	ldr r1, =__bss_start
	ldr r2, =__bss_size
3:	cbz r2, 4f
	str xzr, [r1], #8
	sub r2, r2, #1
	cbnz r2, 3b @keep going until r2 is zero

4:	bl main_asm @jump to program

	b 1b
