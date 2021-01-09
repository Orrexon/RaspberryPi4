.section .text
.global main_asm

main_asm:
//	bl _blink
	bl uart_init
	ldr w0, =hello_world
	bl uart_writeText

	while_true:
		b while_true

.section .data
hello_world: .asciz "Hello world\r\n"

.section .text
.global uart_init
uart_init:

ldr x0, =uart_init_addr
str lr, [x0]

	//enable UART1
	ldr x0, =AUX_BASE
	ldr x0, [x0]
	ldr x1, =AUX_ENABLES_OFFSET
	ldr x1, [x1]
	add x0, x0, x1
	mov x1, #1
	str x1, [x0]

	//set IER register and cntl register to 0
	ldr x0, =AUX_BASE
	ldr x0, [x0]
	ldr x1, =AUX_MU_IER_REG_OFFSET
	ldr x1, [x1]
	add x0, x0, x1
	mov x1, #0
	str x1, [x0]
	
	ldr x0, =AUX_BASE
	ldr x0, [x0]
	ldr x1, =AUX_MU_CNTL_REG_OFFSET
	ldr x1, [x1]
	add x0, x0, x1
	mov x1, #0
	str x1, [x0]

	//8 bits?? dont quite understand why the number 3.....
	ldr x0, =AUX_BASE
	ldr x0, [x0]
	ldr x1, =AUX_MU_LCR_REG_OFFSET
	ldr x1, [x1]
	add x0, x0, x1
	mov x1, #3
	str x1, [x0]
	
	ldr x0, =AUX_BASE
	ldr x0, [x0]
	ldr x1, =AUX_MU_MCR_REG_OFFSET
	ldr x1, [x1]
	add x0, x0, x1
	mov x1, #0
	str x1, [x0]
	
	ldr x0, =AUX_BASE
	ldr x0, [x0]
	ldr x1, =AUX_MU_IER_REG_OFFSET
	ldr x1, [x1]
	add x0, x0, x1
	mov x1, #0
	str x1, [x0]
	
	//disable interrupts
	ldr x0, =AUX_BASE
	ldr x0, [x0]
	ldr x1, =AUX_MU_IIR_REG_OFFSET
	ldr x1, [x1]
	add x0, x0, x1
	mov x1, #0xC6
	str x1, [x0]

	
	ldr x0, =AUX_BASE
	ldr x0, [x0]
	ldr x1, =AUX_MU_BAUD_REG_OFFSET
	ldr x1, [x1]
	add x0, x0, x1
	ldr x1, =AUX_UART_CLOCK
	ldr x1, [x1]
	ldr x2, =115200
	ldr x2, [x2]
//	mov x2, #115200
	mov x3, #8
	mul x4, x2, x3
	udiv x2, x1, x4
	sub x2, x2, #1
	str x2, [x0]

	//set pin 14 and 15 to pull none and then alt function 5
	mov x0, #14
	ldr x1, =PULL_NONE
	ldr x1, [x1]
	ldr x2, =PERIPH_BASE
	ldr x2, [x2]
	ldr x3, =GPPUPPDN0_OFF
	ldr x3, [x3]
	add x2, x2, x3
	mov x3, #1
	ldr x4, =GPIO_MAX_PIN
	ldr x4, [x4]

//make function and call it here
//function body:
	lsl x5, x3, #2
	sub x5, x5, #1

//implement the following 2 rows some day when a more secure function is required
//if (pin_number > field_max) return 0;
//if (value > field_mask) return 0;
	mov x3, #32
	mov x6, #2
//num_fields x7
	udiv x7, x3, x6
//pin_number x0 / num_fields x7
	udiv x8, x0, x7
//reg = base + x8
	add x9, x2, x8
//reg *= 4
	mov x15, #4
	mul x9, x9, x15
//shift = (pin_number % num_fields) remainder-> ((x11 = pin_numberx0 - (x8 * num_fieldsx7)
	msub x11, x8, x7, x0
//shift *= field_size
	mov x15, #2
	mul x11, x11, x15
//curval = *reg
	ldr x12, [x9]
	lsl x13, x5, x11
	bic x12, x12, x13
	lsl x14, x1, x11
	orr x12, x12, x14
	str x12, [x9]
	
//set pin to alt function5
	mov x0, #14
	ldr x1, =GPIO_FUNCTION_ALT5
	ldr x1, [x1]
	ldr x2, =PERIPH_BASE
	ldr x2, [x2]
	ldr x3, =GPFSEL0_OFF
	ldr x3, [x3]
	add x2, x2, x3
	mov x3, #1
	ldr x4, =GPIO_MAX_PIN
	ldr x4, [x4]

//make function and call it here
//function body:
	lsl x5, x3, #3
	sub x5, x5, #1

//implement the following 2 rows some day when a more secure function is required
//if (pin_number > field_max) return 0;
//if (value > field_mask) return 0;
	mov x3, #32
	mov x6, #3
//num_fields x7
	udiv x7, x3, x6
//pin_number x0 / num_fields x7
	udiv x8, x0, x7
//reg = base + x8
	add x9, x2, x8
//reg *= 4
	mov x15, #4
	mul x9, x9, x15
//shift = (pin_number % num_fields) remainder-> ((x11 = pin_numberx0 - (x8 * num_fieldsx7)
	msub x11, x8, x7, x0
//shift *= field_size
	mov x15, #3
	mul x11, x11, x15
//curval = *reg
	ldr x12, [x9]
	lsl x13, x5, x11
	bic x12, x12, x13
	lsl x14, x1, x11
	orr x12, x12, x14
	str x12, [x9]

	//set pin 14 and 15 to pull none and then alt function 5
	mov x0, #15
	ldr x1, =PULL_NONE
	ldr x1, [x1]
	ldr x2, =PERIPH_BASE
	ldr x2, [x2]
	ldr x3, =GPPUPPDN0_OFF
	ldr x3, [x3]
	add x2, x2, x3
	mov x3, #1
	ldr x4, =GPIO_MAX_PIN
	ldr x4, [x4]

//make function and call it here
//function body:
	lsl x5, x3, #2
	sub x5, x5, #1

//implement the following 2 rows some day when a more secure function is required
//if (pin_number > field_max) return 0;
//if (value > field_mask) return 0;
	mov x3, #32
	mov x6, #2
//num_fields x7
	udiv x7, x3, x6
//pin_number x0 / num_fields x7
	udiv x8, x0, x7
//reg = base + x8
	add x9, x2, x8
//reg *= 4
	mov x15, #4
	mul x9, x9, x15
//shift = (pin_number % num_fields) remainder-> ((x11 = pin_numberx0 - (x8 * num_fieldsx7)
	msub x11, x8, x7, x0
//shift *= field_size
	mov x15, #2
	mul x11, x11, x15
//curval = *reg
	ldr x12, [x9]
	lsl x13, x5, x11
	bic x12, x12, x13
	lsl x14, x1, x11
	orr x12, x12, x14
	str x12, [x9]
	
//set pin to alt function5
	mov x0, #15
	ldr x1, =GPIO_FUNCTION_ALT5
	ldr x1, [x1]
	ldr x2, =PERIPH_BASE
	ldr x2, [x2]
	ldr x3, =GPFSEL0_OFF
	ldr x3, [x3]
	add x2, x2, x3
	mov x3, #1
	ldr x4, =GPIO_MAX_PIN
	ldr x4, [x4]

//make function and call it here
//function body:
	lsl x5, x3, #3
	sub x5, x5, #1

//implement the following 2 rows some day when a more secure function is required
//if (pin_number > field_max) return 0;
//if (value > field_mask) return 0;
	mov x3, #32
	mov x6, #3
//num_fields x7
	udiv x7, x3, x6
//pin_number x0 / num_fields x7
	udiv x8, x0, x7
//reg = base + x8
	add x9, x2, x8
//reg *= 4
	mov x15, #4
	mul x9, x9, x15
//shift = (pin_number % num_fields) remainder-> ((x11 = pin_numberx0 - (x8 * num_fieldsx7)
	msub x11, x8, x7, x0
//shift *= field_size
	mov x15, #3
	mul x11, x11, x15
//curval = *reg
	ldr x12, [x9]
	lsl x13, x5, x11
	bic x12, x12, x13
	lsl x14, x1, x11
	orr x12, x12, x14
	str x12, [x9]
//now write to aux_mu_cntl_reg to enable RX/TX
	ldr x0, =AUX_BASE
	ldr x0, [x0]
	ldr x1, =AUX_MU_CNTL_REG_OFFSET
	ldr x1, [x1]
	add x2, x0, x1
	mov x15, #3
	str x15, [x2]	

ldr lr, =uart_init_addr
ldr lr, [lr]
ret lr


.global uart_writeText
uart_writeText:
ldr x15, =uart_writeText_addr
str lr, [x15]
	_while_buffer:
		_while_write_not_ready:
			ldr x1, =AUX_BASE
			ldr x1, [x1]
			ldr x2, =AUX_MU_LSR_REG_OFFSET
			ldr x2, [x2]
			add x3, x1, x2
			and x4, x3, #0x20
			CBZ x4, _while_write_not_ready
 
		ldr x1, =AUX_BASE
		ldr x1, [x1]
		ldr x2, =AUX_MU_IO_REG_OFFSET
		ldr x2, [x2]
		add x3, x1, x2
		ldr x15, [x0]
		str x15, [x3]
		add x0, x0, #1
		ldr x15, [x0]
		CBZ x15, _while_buffer
ldr lr, =uart_writeText_addr
ldr lr, [lr]
ret lr

.section .data
.balign 4
uart_init_addr: .word 0
.balign 4
uart_writeText_addr: .word 0
//extract to another file
//GPIO
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

//UART
.balign 4
AUX_BASE: .word 0xFE215000
.balign 4
AUX_ENABLES_OFFSET: .word 4
.balign 4
AUX_MU_IO_REG_OFFSET: .word 64
.balign 4
AUX_MU_IER_REG_OFFSET: .word 68
.balign 4
AUX_MU_IIR_REG_OFFSET: .word 72
.balign 4
AUX_MU_LCR_REG_OFFSET: .word 76
.balign 4
AUX_MU_MCR_REG_OFFSET: .word 80
.balign 4
AUX_MU_LSR_REG_OFFSET: .word 84
.balign 4
AUX_MU_CNTL_REG_OFFSET: .word 96
.balign 4
AUX_MU_BAUD_REG_OFFSET: .word 104
.balign 4
AUX_UART_CLOCK: .word 500000000
.balign 4
//16*1024
AUX_MAX_QUEUE: .word 16384
