.section .text
.global main_asm
.include "ioasm.s" 
main_asm:
	bl uart_init
	ldr x0, =hello_world
	bl uart_writeText

	while_true:
		b while_true

