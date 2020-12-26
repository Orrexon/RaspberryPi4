
.section .text
.global _start
_start:
@	send message to GPU
	mailbox .req r0 @make alias mailbox for r0
	ldr r1, =ARM_PERIF_BASE
	ldr r1, [r1]
	ldr r2, =ARM_VC_BASE
	ldr r2, [r2]
	add mailbox, r1, r2 
	mov r3, #0
	blink$:

		wait1$:
			status .req r1 
			ldr status, [mailbox, #0x38] @ TODO: offset??
			tst status, #0x80000000
			.unreq status @un-alias r1
		bne wait1$


		message .req r1
		ldr message, =PropertyInfo
		add message, #8
		ldr r5, =ARM_VC_OFFSET_1
		ldr r6, [r5]
		str message, [mailbox, r6]
		.unreq message

		bl _wait

		wait2$:
			status .req r1 
			ldr status, [mailbox, #0x38] @ TODO: offset??
			tst status, #0x80000000
			.unreq status @un-alias r1
		bne wait2$

		
		message .req r1
		ldr message, =PropertyInfo
		add message, #8	
		ldr r5, =ARM_VC_OFFSET_1
		ldr r6, [r5]
		str message, [mailbox, r6]
		.unreq message

		bl _wait

	add r3, r3, #1
	cmp r3, #10
	bne blink$
	/*end*/
	.unreq mailbox
	hang:
		b hang @NOTE Oscar: this is an infinite loop, to prevent from crashing the pi

_wait:
	ldr r4, =wait_bu
	str lr, [r4]

	mov r4, #0
	mov r6, #1
	hold_on$:
		add r4, r4, #1
		cmp r4, r6, ROR #12 @almost a milion 
	bne hold_on$

	ldr lr, =wait_bu
	ldr lr, [lr]
	bx lr

.section .data
.align 4 
PropertyInfo:
.word PropertyInfoEnd - PropertyInfo
.word 0

.word 0x00038041
.word 8
.word 8

.word 130
.word 1

.word 0
PropertyInfoEnd:

.balign 4
wait_bu: .word 0

.balign 4
ARM_PERIF_BASE: .word 0xFE000000
.balign 4
ARM_VC_BASE: .word 0xB880
.balign 4
ARM_VC_OFFSET_1: .word 0x20

.balign 4
ARMC_BASE: .word 0x7e00b000

.balign 4 
M_BOX_ARM_LOCAL_BASE: .word 0x4C000000
.balign 4
M_BOX_ARM_LOCAL_BASE_LOW: .word 0x0FF80000
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

