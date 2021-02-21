#include "ioc.h"

#define PAGESIZE  4096
//granularity
#define PT_PAGE	  0b11 	   // 4K granule
#define PT_BLOCK  0b01 	   // 2M granule
//accessability
#define PT_KERNEL (0<<6)   // privilaged, supervisor EL1 access only
#define PT_USER	  (1<<6)   // unprivilaged EL0 access allowed	
#define PT_RW	  (0<<7)   // read-write
#define PT_RO	  (1<<7)   // read-only
#define PT_AF	  (1<<10)  // accessed flag
#define PT_NX	  (1UL<<54)// no execute
//shareablilty
#define PT_OSH 	  (2<<8)   // outer sharable
#define PT_ISH	  (3<<8)   // inner sharable
//defined in MAIR register
#define PT_MEM	  (0<<2)   // normal memory
#define PT_DEV	  (1<<2)   // device MMIO
#define PT_NC	  (2<<2)   // non cacheable

#define TTBR_ENABLE  1

//get addresses from linker
extern volatile unsigned char _data;
extern volatile unsigned char _end;

//set up translation tables and enable virtual memory
void MMUInit()
{
	unsigned long DataPage = (unsigned long)&_data/PAGESIZE;
	unsigned long Base = PERIPHERAL_BASE >> 21;
	unsigned long* Paging = (unsigned long*)&_end;

	//TTBR0, identity L1
	Paging[0] = (unsigned long)((unsigned char*) &_end + 2*PAGESIZE) | //physical address
		PT_PAGE | 	//it has the "Present flag" must be set and we have area in it mapped by pages
		PT_AF | 	//accessed flag. without this, we'll get Data Abort exception
		PT_USER | 	//non privilaged
		PT_ISH | 	//inner sharable
		PT_MEM; 	//normal memory
	
	//identity L2, first 2M block
	Paging[2*512] = (unsigned long)((unsigned char*) &_end + 3*PAGESIZE) | //physical address
		PT_PAGE | 	//we have area in it mapped by pages
		PT_AF | 	//accessed flag
		PT_USER | 	//non privilaged
		PT_ISH | 	//inner sharable
		PT_MEM; 	//normal memory
	
	//identity L2 2M blocks
	for (int index = 1; index < 512; ++index)
	{
		Paging[2*512 + index] = (unsigned long)(index << 21) | //physical address
			PT_BLOCK | 	//map 2M block
			PT_AF |		//accessed flag
			PT_NX |		//no execute
			PT_USER |	//npon privilaged
			(index >= Base ? PT_OSH | PT_DEV 
			 : PT_ISH | PT_MEM); // different attributes for devi9ce memory
	}


	//identity L3
	for (int index = 0; index < 512; ++index)
	{
		Paging[3*512 + index] = (unsigned long)(index * PAGESIZE) | //physical
			PT_PAGE |	//map 4k page
			PT_AF |		//accessed flag
			PT_USER |	// non privilaged
			PT_ISH |	//inner sharable
			((index < 0x80 || index >= DataPage) ? PT_RW | PT_NX 
			 : PT_RO); //different for code and data
	}



	//TTBR1, kernel L1
	Paging[512+511] = (unsigned long)((unsigned char*) &_end + 4*PAGESIZE) | //physical address
		PT_PAGE | 	//we have area in it mapped by pages
		PT_AF | 	//accessed flag
		PT_KERNEL | 	//privilaged
		PT_ISH | 	//inner sharable
		PT_MEM; 	//normal memory
	
	//kernel L2
	Paging[4*512+511] = (unsigned long)((unsigned char*) &_end + 5*PAGESIZE) | //physical address
		PT_PAGE | 	//we have area in it mapped by pages
		PT_AF | 	//accessed flag
		PT_KERNEL | 	//privilaged
		PT_ISH | 	//inner sharable
		PT_MEM; 	//normal memory
	
	//kernel L3
	Paging[5*512] = (unsigned long)(PERIPHERAL_BASE + 0x215000) | //physical address
		PT_PAGE | 	//we have area in it mapped by pages
		PT_AF |		//accessed flag
		PT_NX |		//no execute
		PT_KERNEL | 	//privilaged
		PT_OSH | 	//outer sharable
		PT_DEV; 	//device memory

	//setting system registers to enable mmu
	//check for 4k granule and at least 36 bits of physical address bus
	unsigned long reg;
	asm volatile ("mrs %0, id_aa64mmfr0_el1" : "=r" (reg));
	//TODO Oscar: print the actual specs found in this register. specs found here: 
	//https://developer.arm.com/docs/ddi0595/c/aarch64-system-registers/id_aa64mmfr0_el1
	//extract the number to string parser and the concatinator and start building a string
	//file. don't get too crazy on the string functions as this won't be a stringy OS as such
	//make it the next video
	Base = reg&0xF;
	if(reg & (0xF << 28)/*4k*/ || Base < 1 /*1=36 bits*/)
	{
		WriteTextUart("Error: 4k granule or 36 bit address not supported\n");
		return;
	}

	//first set memory attributes array, indexed by PT_MEM, PT_DEV, PT_NC
	reg = 	(0xFF << 0)|	//AttrIdx=0: normal, IWBWA, OWBWA, NTR
		(0X04 << 8)|	//AttrIdx=1: devoce, nGnRE (must be OSH too)
		(0x44 << 16);	//AttrIdx=2: non chacheable
	asm volatile ("msr mair_el1, %0" : : "r" (reg));

	//next specify mapping characteristics in translate control register
	reg = 	(0b00LL << 37) |	//TBI=0, no tagging
		(Base << 32) |		//IPS=autodetected
		(0b10LL << 30) |	//TG1=4k
		(0b11LL << 28) |	//SH1=3 inner
		(0b01LL << 26) |	//ORGN1=1 write back
		(0b01LL << 24) |	//ORGN1=1 write back
		(0b0LL  << 23) |	//EPD1 enable higher half
		(25LL	<< 16) |	//T1SZ=25 3 levels (512G)
		(0b00LL << 14) |	//TG0=4k
		(0b11LL << 12) |	//SHO=3 inner
		(0b01LL << 10) |	//ORGN0=1 write back
		(0b01LL << 8)  |	//IRGN0=1 write back
		(0b0LL  << 7)  |	//EPD0 enable lower half
		(25LL   << 0);		//T0SZ=25 3 levels (512G)
	asm volatile ("msr tcr_el1, %0; isb" : : "r" (reg));

	//tell the mmu where our translation tables are. TTBR_ENABLE bit not documented but required
	//lower half: user space
	asm volatile ("msr ttbr0_el1, %0" : : "r" ((unsigned long)&_end + TTBR_ENABLE));
	//higher half: kernel space
	asm volatile ("msr ttbr1_el1, %0" : : "r" ((unsigned long)&_end + TTBR_ENABLE + PAGESIZE));

	//finally toggle some bits in system control register to enable page translation
	asm volatile ("dsb ish; isb; mrs %0, sctlr_el1" : "=r" (reg));
	reg |= 0xC00800; 	//set mandatory reserved bits
	reg &= 	~((1 << 25) |	//clear EE, little endian translation tables 
		  (1 << 24) |	//clear E0E
		  (1 << 19) |	//clear WXN
		  (1 << 12) |	//clear I, no instruction cach
		  (1 << 4)  |	//clear SA0
		  (1 << 3)  |	//clear SA
		  (1 << 2)  |	//clear C, no cache at all
		  (1 << 1));	//clear A, no alignment check
	reg |= (1 << 0);	//set M, enable MMU
	asm volatile ("msr sctlr_el1, %0; isb" : : "r" (reg));
}
