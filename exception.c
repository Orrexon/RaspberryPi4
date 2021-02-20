#include "ioc.h"
#include "string_util.h"

//extract these typedefs: 
typedef unsigned long u64;
typedef unsigned char u8;
typedef unsigned int  u32;
typedef unsigned short u16;

typedef long i64;
typedef char i8;
typedef int  i32;
typedef short i16;


void RunHandler(u64 type,
		u64 esr,
		u64 elr,
		u64 spsr,
		u64 far)
{
	switch(type)
	{
		case 0: 
			{
				WriteTextUart("Synchronous");
				break;
			}
		case 1: 
			{
				WriteTextUart("IRQ");
				break;
			}
		case 2: 
			{
				WriteTextUart("FIQ");
				break;
			}
		case 3: 
			{
				WriteTextUart("SError");
				break;
			}
	}
	WriteTextUart(": ");
	//exception types, there are more in the arm docs
	switch(esr >> 26)
	{
		case 0b000000:
			{
				WriteTextUart("Unknown");
				break;
			}
		case 0b000001:
			{
				WriteTextUart("Trapped WFI/WFE");
				break;
			}
		case 0b001110:
			{
				WriteTextUart("Illegal execution");
				break;
			}
		case 0b010101:
			{
				WriteTextUart("System Call");
				break;
			}
		case 0b100000:
			{
				WriteTextUart("Instruction abort, lower EL");
				break;
			}
		case 0b100001:
			{
				WriteTextUart("Instruction abort, same EL");
				break;
			}
		case 0b100010:
			{
				WriteTextUart("Instruction alignment fault");
				break;
			}
		case 0b100100:
			{
				WriteTextUart("Data abort, lower EL");
				break;
			}
		case 0b100101:
			{
				WriteTextUart("Data abort, same EL");
				break;
			}
		case 0b100110:
			{
				WriteTextUart("Stack alignment fault");
				break;
			}
		case 0b101100:
			{
				WriteTextUart("floating point");
				break;
			}
		default:
			{
				WriteTextUart("Unknown");
				break;
			}
	}

	//Data abort cause
	if(esr >> 26 == 0b100100 || esr >> 26 == 0b100101)
	{
		WriteTextUart(", ");
		switch((esr >> 2) & 0x3)
		{
			case 0:
				{
					WriteTextUart("Address size fault");
					break;
				}
			case 1:
				{
					WriteTextUart("Translation fault");
					break;
				}
			case 2:
				{
					WriteTextUart("Access flag fault");
					break;
				}
			case 3:
				{
					WriteTextUart("Permission fault");
					break;
				}
		}
		
		switch(esr & 0x3)
		{
			case 0:
				{
					WriteTextUart(" at level 0");
					break;
				}
			case 1:
				{
					WriteTextUart(" at level 1");
					break;
				}
			case 2:
				{
					WriteTextUart(" at level 2");
					break;
				}
			case 3:
				{
					WriteTextUart(" at level 3");
					break;
				}
		}
	}

	//print registers
	WriteTextUart(":\n ESR_EL1 ");
	WriteTextUart(parse_ulong(esr >> 32, 16));
	WriteTextUart(parse_ulong(esr, 16));
	WriteTextUart(" ELR_EL1");
	WriteTextUart(parse_ulong(elr >> 32, 16));
	WriteTextUart(parse_ulong(elr, 16));
	WriteTextUart("\n SPSR_EL1 ");
	WriteTextUart(parse_ulong(spsr >> 32, 16));
	WriteTextUart(parse_ulong(spsr, 16));
	WriteTextUart(" FAR_EL1 ");
	WriteTextUart(parse_ulong(far >> 32, 16));
	WriteTextUart(parse_ulong(far, 16));
	WriteTextUart("\n");

	//Handle it properly asap! 
	while(1);

}
