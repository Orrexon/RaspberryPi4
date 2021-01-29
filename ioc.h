#define PERIPHERAL_BASE 0xFE000000

void uart_init_c();
void uart_write_text_c(char* buffer);
void uart_loadOutputFifo();
unsigned char uart_readByte();
unsigned int uart_isReadByteReady();
void uart_writeByteBlocking(unsigned char ch);
void uart_update();
void WriteMMIO(long reg, unsigned int val);
unsigned int ReadMMIO(long reg);
