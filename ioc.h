#if !defined IOC_H
#define PERIPHERAL_BASE 0xFE000000
#define LEGACY_PERIPHERAL_BASE 0x7E000000

void uart_init_c();
void WriteTextUart(char* buffer);
void uart_loadOutputFifo();
unsigned char uart_readByte();
unsigned int uart_isReadByteReady();
void WriteByteQueue(unsigned char ch);
void uart_update();
void WriteMMIO(long reg, unsigned int val);
unsigned int ReadMMIO(long reg);
void ReadAndWriteByte();

#define IOC_H
#endif
