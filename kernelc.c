#include "ioc.h"
#include "framebuffer.h"
#include "timing.h"

void main()
{
    uart_init_c();
    fb_init();

    drawRect(25, 25, 400, 300, 0x07, 0);
    drawRect(100, 100, 50, 75, 0x37, 1);

    drawCircle(550, 480, 300, 0x0A, 0);
    drawCircle(55, 48, 30, 0xA2, 1);

    drawPixel(700, 700, 0x09);

    drawChar('P', 700, 700, 0x03);
    drawString(800, 800, "Look I am text!", 0x0E);

    unsigned long count = cntpct_el0();
    char countString[4] = {count & 0xFF000000, count & 0x00FF0000, count & 0x0000FF00, count & 0x000000FF };
    drawString(1000, 1000, &countString[0], 0x0F);

    while (1);
}
