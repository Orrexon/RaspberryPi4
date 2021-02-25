.data

.equ LOCAL_CONTROL, 	0xff800000
.equ LOCAL_PRESCALER,	0xff800008
.equ OSC_FREQ,        	54000000
.equ MAIN_STACK,      	0x400000

.section ".text.boot"  // Make sure the linker puts this at the start of the kernel image

.global _start  // Execution starts here

_start:
    ldr     x0, =LOCAL_CONTROL   // Sort out the timer
    str     wzr, [x0]
    mov     w1, 0x80000000
    str     w1, [x0, #(LOCAL_PRESCALER - LOCAL_CONTROL)]

    ldr     x0, =OSC_FREQ
    msr     cntfrq_el0, x0
    msr     cntvoff_el2, xzr

    // Check processor ID is zero (executing on main core), else hang
    mrs     x1, mpidr_el1
    and     x1, x1, #3
    cbz     x1, TWO

    // We're not on the main core, so hang in an infinite wait loop
    adr     x5, spin_cpu0
ONE:  wfe
    ldr     x4, [x5, x1, lsl #3]
    cbz     x4, ONE

    ldr     x2, =__stack_start   // Get ourselves a fresh stack - location depends on CPU core asking
    lsl     x1, x1, #9           // Multiply core_number by 512
    add     x3, x2, x1           // Add to the address
    mov     sp, x3

    mov     x0, #0
    mov     x1, #0
    mov     x2, #0
    mov     x3, #0
    br      x4
    b       ONE
TWO:  // We're on the main core!

    // Set stack to start somewhere safe
    mov     sp, #MAIN_STACK

    // Clean the BSS section
    ldr     x1, =__bss_start     // Start address
    ldr     w2, =__bss_size      // Size of the section
THREE:  cbz     w2, FOUR               // Quit loop if zero
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, THREE               // Loop if non-zero

    // Jump to our main() routine in C (make sure it doesn't return)
FOUR:  bl      main
    // In case it does return, halt the master core too
    b       ONE

.ltorg

.org 0xd8
.globl spin_cpu0
spin_cpu0:
        .quad 0

.org 0xe0
.globl spin_cpu1
spin_cpu1:
        .quad 0

.org 0xe8
.globl spin_cpu2
spin_cpu2:
        .quad 0

.org 0xf0
.globl spin_cpu3
spin_cpu3:
        .quad 0



















/*.section ".text.boot"

.global _start

_start:
    // read cpu id, stop slave cores 
    mrs     x1, mpidr_el1
    and     x1, x1, #3 
    cbz     x1, TWO
    // cpu id > 0, stop
ONE:  wfe
    b       ONE
TWO:  // cpu id == 0

    // set stack before our code
    ldr     x1, =_start

    // set up EL1
    mrs     x0, CurrentEL
    and     x0, x0, #0b1100 //12, clear reserved bits

    // running at EL3?
    cmp     x0, #0b1100 //12, meaning EL3
    bne     FIVE1
    // should never be executed, just for completeness
    mov     x2, #0x1B1 	//in cortex-A72 bit 10 and 11 are res 0, 
			//should be a cortex-a53 thing-> #0x5b1
			//https://developer.arm.com/documentation/100095/0003/system-control/aarch32-register-descriptions/secure-configuration-register
    msr     scr_el3, x2
    mov     x2, #0x3c9 //seems OK WHat is the difference between ELXt and ELXh????
    msr     spsr_el3, x2
    adr     x2, FIVE1
    msr     elr_el3, x2
    eret

    // running at EL1?
FIVE1:  cmp     x0, #0b100 //4, bit 2 set meaning EL1!!
    beq     FIVE2
    msr     sp_el1, x1
    // enable CNTP for EL1
    mrs     x0, cnthctl_el2
    orr     x0, x0, #3
    msr     cnthctl_el2, x0
    msr     cntvoff_el2, xzr
    // enable AArch64 in EL1
    mov     x0, #(1 << 31)      // AArch64
    orr     x0, x0, #(1 << 1)   // SWIO hardwired on Pi3 also rpi4 it would seem
    msr     hcr_el2, x0
    mrs     x0, hcr_el2		//why is this loaded back into x0? it should already be what it set the hcr_el2 to ???? 
    // Setup SCTLR access
    mov     x2, #0x0800 
    movk    x2, #0x30d0, lsl #16
    msr     sctlr_el1, x2
    // set up exception handlers
    ldr     x2, =_vectors
    msr     vbar_el1, x2
    // change execution level to EL1
    mov     x2, #0x3c4
    msr     spsr_el2, x2
    adr     x2, FIVE2
    msr     elr_el2, x2
    eret

FIVE2:  mov     sp, x1

    // clear bss
    ldr     x1, =__bss_start
    ldr     w2, =__bss_size
THREE:  cbz     w2, FOUR
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, THREE

FOUR:	
	#jump to program
//	bl main_asm
	bl main
	b ONE












/*
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

FIVE1:  cmp     x0, #4
    	beq     FIVE2
    	msr     sp_el1, x1
    	// enable CNTP for EL1
   	mrs     x0, cnthctl_el2
   	orr     x0, x0, #3
   	msr     cnthctl_el2, x0
    	msr     cntvoff_el2, xzr
   	// enable AArch64 in EL1
   	mov     x0, #(1 << 31)      // AArch64
    	orr     x0, x0, #(1 << 1)   // SWIO hardwired on Pi3
   	msr     hcr_el2, x0
    	mrs     x0, hcr_el2

//enable some things in the system control register
	mov x2, #0x0800
	movk x2, #0x30D0, lsl #16
	msr sctlr_el1, x2

	//exception handlers
	ldr x2, =_vectors
	msr vbar_el1, x2

	mov x2, #0x3C4
	msr spsr_el2, x2
	
	adr x2, FIVE2
	msr elr_el2, x2
	eret

FIVE2:	mov sp, x1
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





.align 11
_vectors:
//synchronous
.align 7
	mov x0, #0
	mrs x1, esr_el1
	mrs x2, elr_el1
	mrs x3, spsr_el1
	mrs x4, far_el1
	b RunHandler

//IRQ
.align 7
	mov x0, #1
	mrs x1, esr_el1
	mrs x2, elr_el1
	mrs x3, spsr_el1
	mrs x4, far_el1
	b RunHandler
	
//FIQ
.align 7
	mov x0, #2
	mrs x1, esr_el1
	mrs x2, elr_el1
	mrs x3, spsr_el1
	mrs x4, far_el1
	b RunHandler

//SError
.align 7
	mov x0, #3
	mrs x1, esr_el1
	mrs x2, elr_el1
	mrs x3, spsr_el1
	mrs x4, far_el1
	b RunHandler

*/
