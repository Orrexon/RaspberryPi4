#include "ioc.h"
#include "framebuffer.h"

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

    while (1);
}
