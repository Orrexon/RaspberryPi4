#include "ioc.h"
#include "framebuffer.h"
#include "timing.h"


//move this baby somewhere
char* parse_ulong(int num, int base_)
{
	if(base_ < 2)
	{
		base_ = 2;
	}
	else if(base_ > 16)
	{
		base_ = 16;
	}
    	static char result[32] = {0}; //this will be static (local persist) until I have "malloc". unless I keep it like this
    	char *ptr1 = &result[0], tmp_char;
    	int tmp_value;
    	int base = base_;

	//TODO Oscar: add support for the character '.' to be able to print floats
    	int index = 0;
    	do {
        	tmp_value = num;
        	num /= base;
        	result[index++] = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - num * base)];
    	} while ( num );

    	// Apply negative sign
    	if (tmp_value < 0) result[index++] = '-';
    	result[index--] = '\0';
    	while(ptr1 < &result[index]) {
        	tmp_char = result[index];
        	result[index--]= *ptr1;
        	*ptr1++ = tmp_char;
    	}
    	return &result[0];
}

unsigned char getKey()
{
	unsigned char Result = 0;
	if(uart_isReadByteReady())
	{
		Result = uart_readByte();
	}
	return Result;
}


void ConCat(char* first, char* second)
{
	int countFirst = 0;
	while(*(first + countFirst) != '\0')
	{
		++countFirst;
	}

	int countSecond = 0;
	while(*(second + countSecond) != '\0')
	{
		*(first + countFirst) = *(second + countSecond);
		++countFirst;
		++countSecond;	
	}

	*(first + countFirst) = '\0';
}


void main()
{
	//initializations
    	uart_init_c();
    	FrameBufferInit();
    	timing_init();
    	unsigned long countFirst = millisec_count();
	//testing the uart again like before
	uart_write_text_c("This should be visible in my debug terminal!\r\n"); 

	//"start screen"
	
	drawRect(200, 25, 700, 800, 0x0A, 0);
    	drawString(500, 500, "This is STARTSCREEN!", 0x0A, 5);
	uart_write_text_c("waiting for key press\r\n"); 
	while(!getKey());
	uart_write_text_c("key pressed, start loop\r\n");
	
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
		ConCat( deltaFromFuncString, "\n"); 
		uart_write_text_c(deltaFromFuncString);

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
			uart_write_text_c("key pressed 's', end loop\r\n");
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
