
.section .text
.global _blink
_blink:
	ldr x4, =blink_buffer
	str lr, [x4]
//	send message to GPU
//	# make alias mailbox for r0
	mailbox .req x0 
	ldr x1, =ARM_PERIF_BASE 
	ldr x1, [x1]
	ldr x2, =ARM_VC_BASE
	ldr x2, [x2]
	add mailbox, x1, x2 
	mov w3, #0
	blink$:

		wait1$:
			status .req x1 
			ldr status, [mailbox, #0x38] 
			tst status, #0x80000000
			.unreq status 
		bne wait1$


		message .req x1
		ldr message, =PropertyInfo_ON
		add message, message, #8
		ldr x5, =ARM_VC_OFFSET_1
		ldr x6, [x5]
		str message, [mailbox, x6]
		.unreq message

		bl _wait

		wait2$:
			status .req x1 
			ldr status, [mailbox, #0x38] 
			tst status, #0x80000000
			.unreq status 
		bne wait2$

		
		message .req x1
		ldr message, =PropertyInfo_OFF
		add message, message, #8	
		ldr x5, =ARM_VC_OFFSET_1
		ldr x6, [x5]
		str message, [mailbox, x6]
		.unreq message

		bl _wait

	add x3, x3, #1
	cmp x3, #30
	bne blink$
//	/*end*/
	.unreq mailbox
	ldr lr, =blink_buffer
	ldr lr, [lr]
	ret lr



_wait:
	ldr x4, =wait_buffer
	str lr, [x4]

	mov x4, #0
	mov x6, #1
	hold_on$:
		add x4, x4, #1
		ROR x6,x6, #12
		cmp x4, x6 
	bne hold_on$

	ldr lr, =wait_buffer
	ldr lr, [lr]
	ret lr

.section .data
.align 4 
PropertyInfo_ON:
.word PropertyInfoEnd_ON - PropertyInfo_ON
.word 0

.word 0x00038041
.word 8
.word 8

.word 130
.word 1

.word 0
PropertyInfoEnd_ON:

PropertyInfo_OFF:
.word PropertyInfoEnd_OFF - PropertyInfo_OFF
.word 0

.word 0x00038041
.word 8
.word 8

.word 130
.word 0

.word 0
PropertyInfoEnd_OFF:

.balign 4
wait_buffer: .word 0
.balign 4
blink_buffer: .word 0

.balign 4
ARM_PERIF_BASE: .word 0xFE000000
.balign 4
ARM_VC_BASE: .word 0xB880

.balign 4
ARM_VC_OFFSET_1: .word 0x20
.balign 4
ARMC_BASE: .word 0x7e00b000
.balign 4
ARMC_IRQ0_SET: .word 0x210
.balign 4
ARMC_IRQ0_CLR: .word 0x220

##@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
##@ARM core mailboxes
##@TODO Oscar: base addresses are defined in 35 bit, (an extra hex-digit) 
##@not 32 bit, according to documentation.
##@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//'full 35 bit' addr 0x4_C000_0000 
.balign 4 
M_BOX_ARM_LOCAL_BASE: .word 0xC0000000 

//full 35 addr 0xF_F800_0000 
.balign 4
M_BOX_ARM_LOCAL_BASE_LOW: .word 0xF8000000 
.balign 4
M_BOX_OFFSET_0: .word 0x80
.balign 4
M_BOX_OFFSET_1: .word 0x84
.balign 4
M_BOX_OFFSET_2: .word 0x88
.balign 4
M_BOX_OFFSET_3: .word 0x8C
.balign 4
M_BOX_OFFSET_4: .word 0x90
.balign 4
M_BOX_OFFSET_5: .word 0x94
.balign 4
M_BOX_OFFSET_6: .word 0x98
.balign 4
M_BOX_OFFSET_7: .word 0x9C
.balign 4
M_BOX_OFFSET_8: .word 0xA0
.balign 4
M_BOX_OFFSET_9: .word 0xA4
.balign 4
M_BOX_OFFSET_10: .word 0xA8
.balign 4
M_BOX_OFFSET_11: .word 0xAC
.balign 4
M_BOX_OFFSET_12: .word 0xB0
.balign 4
M_BOX_OFFSET_13: .word 0xB4
.balign 4
M_BOX_OFFSET_14: .word 0xB8
.balign 4
M_BOX_OFFSET_15: .word 0xBC
.balign 4
M_BOX_CLR_OFFSET_0: .word 0xC0
.balign 4
M_BOX_CLR_OFFSET_1: .word 0xC4
.balign 4
M_BOX_CLR_OFFSET_2: .word 0xC8
.balign 4
M_BOX_CLR_OFFSET_3: .word 0xCC
.balign 4
M_BOX_CLR_OFFSET_4: .word 0xD0
.balign 4
M_BOX_CLR_OFFSET_5: .word 0xD4
.balign 4
M_BOX_CLR_OFFSET_6: .word 0xD8
.balign 4
M_BOX_CLR_OFFSET_7: .word 0xDC
.balign 4
M_BOX_CLR_OFFSET_8: .word 0xE0
.balign 4
M_BOX_CLR_OFFSET_9: .word 0xE4
.balign 4
M_BOX_CLR_OFFSET_10: .word 0xE8
.balign 4
M_BOX_CLR_OFFSET_11: .word 0xEC
.balign 4
M_BOX_CLR_OFFSET_12: .word 0xF0
.balign 4
M_BOX_CLR_OFFSET_13: .word 0xF4
.balign 4
M_BOX_CLR_OFFSET_14: .word 0xF8
.balign 4
M_BOX_CLR_OFFSET_15: .word 0xFC

