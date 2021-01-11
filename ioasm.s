

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
	mov x3, #2
	ldr x4, =GPIO_MAX_PIN
	ldr x4, [x4]


	bl gpio_call
//set pin to alt function5
	mov x0, #14
	ldr x1, =GPIO_FUNCTION_ALT5
	ldr x1, [x1]
	ldr x2, =PERIPH_BASE
	ldr x2, [x2]
	ldr x3, =GPFSEL0_OFF
	ldr x3, [x3]
	add x2, x2, x3
	mov x3, #3
	ldr x4, =GPIO_MAX_PIN
	ldr x4, [x4]

	bl gpio_call

	//set pin 14 and 15 to pull none and then alt function 5
	mov x0, #15
	ldr x1, =PULL_NONE
	ldr x1, [x1]
	ldr x2, =PERIPH_BASE
	ldr x2, [x2]
	ldr x3, =GPPUPPDN0_OFF
	ldr x3, [x3]
	add x2, x2, x3
	mov x3, #2
	ldr x4, =GPIO_MAX_PIN
	ldr x4, [x4]

	bl gpio_call
	
//set pin to alt function5
	mov x0, #15
	ldr x1, =GPIO_FUNCTION_ALT5
	ldr x1, [x1]
	ldr x2, =PERIPH_BASE
	ldr x2, [x2]
	ldr x3, =GPFSEL0_OFF
	ldr x3, [x3]
	add x2, x2, x3
	mov x3, #3
	ldr x4, =GPIO_MAX_PIN
	ldr x4, [x4]

	bl gpio_call

//now write to aux_mu_cntl_reg to enable RX/TX
	ldr x0, =AUX_BASE
	ldr x0, [x0]

	ldr x1, =AUX_MU_CNTL_REG_OFFSET
	ldr x1, [x1]
	add x2, x0, x1
	mov x3, #3
	str x3, [x2]	

ldr lr, =uart_init_addr
ldr lr, [lr]
ret lr


.global gpio_call
gpio_call:
//gpio_call(pin_number, value, base, field_size, field_max)
ldr x15, =gpio_call_addr
str lr, [x15]
	mov x5, #1
	lsl x5, x5, x3
	sub x5, x5, #1

//implement the following 2 rows some day when a more secure function is required
//if (pin_number > field_max) return 0;
//if (value > field_mask) return 0;
	mov x6, #32
//num_fields x7
	udiv x7, x6, x3
//pin_number x0 / num_fields x7
	udiv x8, x0, x7
//  x8 *= 4
	mov x15, #4
	mul x8, x8, x15
//reg = base + x8
	add x9, x2, x8
	
//shift = (pin_number % num_fields) remainder-> ((x11 = pin_numberx0 - (x8 * num_fieldsx7)
	msub x11, x8, x7, x0
//shift *= field_size
	mul x11, x11, x3
//curval = *reg
	ldr x12, [x9]
	lsl x13, x5, x11
	bic x12, x12, x13
	lsl x14, x1, x11
	orr x12, x12, x14
	str x12, [x9]


ldr lr, =gpio_call_addr
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
			ldr x5, [x3]
			and x4, x5, #0x20
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
		CBNZ x15, _while_buffer
ldr lr, =uart_writeText_addr
ldr lr, [lr]
ret lr

.section .data
.balign 4
uart_init_addr: .word 0
.balign 4
uart_writeText_addr: .word 0
.balign 4
gpio_call_addr: .word 0

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



















































