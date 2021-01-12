#include "ioc.h"

void main()
{
    uart_init_c();
    uart_write_text_c("Hello world!\n");
    while (1);
}
