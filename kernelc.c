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

void main()
{
    	uart_init_c();
    	fb_init();
    	timing_init();
    	unsigned long countFirst = millisec_count();
	uart_loadOutputFifo(); 

	//"start screen"
	
	drawRect(200, 25, 700, 800, 0x0A, 0);
    	drawString(500, 500, "This is STARTSCREEN!", 0x0A, 10);
	//while(!getKey());
	uart_loadOutputFifo(); 

    	for(unsigned int i = 0; i < 5000; ++i)
	{
		clear(0x00);
		drawRect(55, 48, 400, 300, 0x08, 0);
    		drawRect(100, 100, 50, 75, 0x52, 1);

	    	drawCircle(550, 480, 300, 0x0A,0);
    		drawCircle(55, 48, 30, 0xA2, 0);


		drawLine(200, 300, 700, 800, 0x06);
    		drawString(800, 800, "Look I am text!", 0x0E, 1);
	    	drawString(800, 1000, "Look I am text!", 0x0E, 2);

    	
		drawRect(900, 25, 300, 800, 0x0A, 0);
	    	drawString(910, 30, "Parsing ints to strings", 0x0F, 1);
		drawRect(900, 900, 300, 180, 0x0A, 0);
	    	drawString(910, 905, "deltatimes in milliseconds", 0x0F, 1);
    	
		unsigned long delta =  millisec_count_delta(countFirst);
    		char* deltaString = parse_ulong(delta, 10);
	    	drawString(1000, 1000, deltaString, 0x0F, 1);
    	
		char* one = parse_ulong(12345, 10);
	    	drawString(1000, 100, one, 0x0F, 1);
    	
		char* two = parse_ulong(78355, 10);
	    	drawString(1000, 200, two, 0x0F, 1);
    	
		char* three = parse_ulong(5500, 10);
	    	drawString(1000, 300, three, 0x0F, 1);
    	
		char* four = parse_ulong(1234567890, 10);
	    	drawString(1000, 400, four, 0x0F, 1);
    	
		unsigned long delta2 =  millisec_count_delta(delta);
	    	char* deltaString2 = parse_ulong(delta2, 10);
    		drawString(1000, 1050, deltaString2, 0x0F, 1);
	}

	clear(0xFF);
    	drawString(500, 500, "This is ENDSCREEN!", 0x0A, 10);
    	while (1);
}
