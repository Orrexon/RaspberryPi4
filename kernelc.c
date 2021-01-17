#include "ioc.h"
#include "framebuffer.h"
#include "timing.h"


//move this baby somewhere
char* parse_ulong(int num)
{
    static char result[32] = {0};
    char *ptr1 = &result[0], tmp_char;
    int tmp_value;
    int base = 10;

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

void main()
{
    uart_init_c();
    fb_init();
    timing_init();
    unsigned long countFirst = millisec_count();
    wait_millisec(5000);

    drawRect(25, 25, 400, 300, 0x07, 0);
    drawRect(100, 100, 50, 75, 0x37, 1);

    wait_millisec(5000);
    drawCircle(550, 480, 300, 0x0A, 0);
    drawCircle(55, 48, 30, 0xA2, 1);

    wait_millisec(5000);
    drawPixel(750, 750, 0x09);

    drawChar('P', 700, 700, 0x03, 1);
    drawString(800, 800, "Look I am text!", 0x0E, 1);

    unsigned long delta =  millisec_count_delta(countFirst);
    char* deltaString = parse_ulong(delta);
    drawString(1000, 1000, deltaString, 0x0F, 1);

    while (1);
}
