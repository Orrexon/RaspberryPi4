#include "ioc.h"
#include "framebuffer.h"
#include "timing.h"

void main()
{
    uart_init_c();
    fb_init();
    timing_init();
    unsigned long countFirst = millisec_count();
    wait_millisec(5000);

    drawRect(25, 25, 400, 300, 0x07, 0);
    drawRect(100, 100, 50, 75, 0x37, 1);

    drawCircle(550, 480, 300, 0x0A, 0);
    drawCircle(55, 48, 30, 0xA2, 1);

    drawPixel(700, 700, 0x09);

    drawChar('P', 700, 700, 0x03, 1);
    drawString(800, 800, "Look I am text!", 0x0E, 1);

    unsigned long delta =  millisec_count_delta(countFirst);
    char deltaString[4] = {((char)(delta << 24) & 0xFF000000), 
	    		   ((char)(delta << 16) & 0x00FF0000), 
	    		   ((char)(delta << 8 ) & 0x0000FF00), 
	    		   ((char)delta & 0x000000FF) };
    drawString(1000, 1000, &deltaString[0], 0x0F, 1);

    while (1);
}
