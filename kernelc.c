#include "ioc.h"
#include "framebuffer.h"
#include "timing.h"
#include "mmu.h"
#include "string_util.h"

unsigned char getKey()
{
	unsigned char Result = 0;
	if(uart_isReadByteReady())
	{
		Result = uart_readByte();
	}
	return Result;
}

 //IO_REGISTER
#define KERNEL_MU_IO  ((volatile unsigned int* )0xFFFFFFFFFFE00040)
//LSR_REGISTER
#define KERNEL_MU_LSR ((volatile unsigned int* )0xFFFFFFFFFFE00054) 

void main()
{
	//initializations
    	uart_init_c();	
	WriteTextUart("uart initialized!\r\n"); 
    	FrameBufferInit();
	WriteTextUart("framebuffer initialized!\r\n"); 
    	timing_init();
	WriteTextUart("timing initialized!\r\n"); 
	MMUInit();
	WriteTextUart("MMU initialized!\r\n"); 
    	unsigned long countFirst = millisec_count();
	//testing the uart again like before
	WriteTextUart("This should be visible in my debug terminal!\r\n"); 
	//testing the mmu
	char* MmuString = "Writing through MMIO mapped in higher half \r\n";
	WriteTextUart("writing through identity mapped MMIO. \r\n");
	while(*MmuString) 
	{
        	WriteTextUart("wait until we can send\r\n");
        	do
		{
			asm volatile("nop");	
        		WriteTextUart("Stuck in the loop, eh? \r\n");
		}while((*(volatile unsigned int* )0xFFFFFFFFFFE00054) & 0x20);//*KERNEL_MU_LSR & 0x20);
        	WriteTextUart("write the character to the buffer\r\n");
        	(*(volatile unsigned int* )0xFFFFFFFFFFE00040) = *MmuString++;//*KERNEL_MU_IO = *MmuString++;
	} //This function exists already. 
	//don't worry all the kernel code will change any way 
	//not sure if this is really how this works though 
	WriteTextUart("ok let's hit it! \r\n");
	for(int i = 0; i < 100; ++i)
	{
		ReadAndWriteByte();
	}
	//"start screen"
	
	drawRect(200, 25, 700, 800, 0x0A, 0);
    	drawString(500, 500, "This is STARTSCREEN!", 0x0A, 5);
	WriteTextUart("waiting for key press\r\n"); 
	while(!getKey());
	WriteTextUart("key pressed, start loop\r\n");
	
	unsigned long deltaFrame = 0;
	int run = 1;
    	while(run)
	{
		unsigned long clearStart = millisec_count();
		Clear(0x00);		
		unsigned long clearEnd = millisec_count();
		unsigned long deltaClear = clearEnd - clearStart;
	    	char* deltaClearString = parse_ulong(deltaClear, 10);
    		drawString(1000, 1000, deltaClearString, 0x0F, 1);
		
		unsigned long deltaFromFunc = millisec_count_delta(clearStart);
		char* deltaFromFuncString = parse_ulong(deltaFromFunc, 10);
		Concat( deltaFromFuncString, "\r\n"); 
		Concat( deltaClearString, "\r\n"); 
		WriteTextUart(deltaFromFuncString);
		WriteTextUart(deltaClearString);
		
		drawRect(55, 48, 400, 300, 0x08, 0);
    		drawRect(100, 100, 50, 75, 0x52, 1);

	    	drawCircle(550, 480, 300, 0x0A,0);
    		drawCircle(55, 48, 30, 0xA2, 0);

		drawLine(200, 300, 700, 800, 0x06);

		drawRect(900, 900, 300, 180, 0x0A, 0);
	    	drawString(910, 905, "deltatimes in milliseconds, clear and frame", 0x0F, 2);
    	
    		char ch = 0;
    		if( (ch = getKey()) )
		{
			if(ch == 's') run = 0; //endloop
			uart_loadOutputFifo();
			WriteTextUart("key pressed 's', end loop\r\n");
		}

		deltaFrame =  millisec_count_delta(deltaFrame);
	    	char* deltaFrameString = parse_ulong(deltaFrame, 10);
    		drawString(1000, 1050, deltaFrameString, 0x0F, 1);
	}

	Clear(0xFF);
    	drawString(500, 500, "This is ENDSCREEN!", 0x0A, 5);
	char* totalCountString = parse_ulong(countFirst, 10);
    	drawString(500, 650, totalCountString, 0x0F, 1);
    	while (1)
	{
		//this little thing should show the characters I press in the debug terminal
		uart_update();
	}
}
