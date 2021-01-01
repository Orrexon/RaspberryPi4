.section ".text.boot"

.global _start

_start:
@check processor ID == 0 to execute on main core

	mrs x1, mpidr_el1
	and x1, x1, #3
	cbz x1, 2f @if it is zero, jump to lbl 2 and clean bss

1:	wfe @else, wait for event
	b 1b

2:
@set stack to start below code
	ldr x1, =_start
	mov sp, r1

@clean the bss
	ldr x1, =__bss_start
	ldr x2, =__bss_size
3:	cbz x2, 4f
	str xzr, [r1], #8
	sub x2, x2, #1
	cbnz x2, 3b @keep going until r2 is zero

4:	bl main_asm @jump to program

	b 1b
