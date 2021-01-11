.section .text
.global main_asm
 
main_asm:
	bl uart_init
	ldr x0, =hello_world
	bl uart_writeText

	while_true:
		b while_true

.section .data
hello_world: .asciz "Hello world\r\n"
