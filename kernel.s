.section .text
.global main_asm

main_asm:
	bl _blink
	bl uart_init
	ldr w1, =hello_world
	bl uart_writeText

	while_true:
		b while_true

.section .data
hello_world: .ascii "Hello world\n"

.section .text
.global uart_init
uart_init:
	

.global uart_writetext
uart_writeText:


.section .data
.balign 4
PERIPH_BASE: 	.word 0xFE000000
.balign 4
GPFSEL0_OFF: 	.word 0x200000
.balign 4
GPSET0_OFF: 	.word 0x20001C
.balign 4
GPCLR0_OFF: 	.word 0x200028
.balign 4
GPPUPPDN0_OFF: 	.word 0x2000E4

.balign 4
GPIO_MAX_PIN: 		.word 53
.balign 4
GPIO_FUNCTION_ALT5: 	.word 2

.balign 4
PULL_NONE: .word 0
