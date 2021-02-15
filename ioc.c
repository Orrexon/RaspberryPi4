 // GPIO

enum 
{
    PERIPHERAL_BASE = 0xFE000000,
    GPFSEL0         = PERIPHERAL_BASE + 0x200000,
    GPSET0          = PERIPHERAL_BASE + 0x20001C,
    GPCLR0          = PERIPHERAL_BASE + 0x200028,
    GPPUPPDN0       = PERIPHERAL_BASE + 0x2000E4
};

enum 
{
    GPIO_MAX_PIN       = 53,
    GPIO_FUNCTION_OUT  = 1,
    GPIO_FUNCTION_ALT5 = 2,
    GPIO_FUNCTION_ALT3 = 7
};

enum 
{
    Pull_None = 0,
    Pull_Down = 1, // these values can be checked with a gllelgel-scope
    Pull_Up   = 2
};

void WriteMMIO(long reg, unsigned int val) 
{ 
	*(volatile unsigned int *)reg = val; 
}

unsigned int ReadMMIO(long reg) 
{
       	return *(volatile unsigned int *)reg; 
}

unsigned int gpio_call(	unsigned int pin_number, 
			unsigned int value, 
			unsigned int base, 
			unsigned int field_size, 
			unsigned int field_max) 
{
    unsigned int field_mask = (1 << field_size) - 1;
  
    if (pin_number > field_max) return 0;
    if (value > field_mask) return 0; 

    unsigned int num_fields = 32 / field_size;
    unsigned int reg = base + ((pin_number / num_fields) * 4);
    unsigned int shift = (pin_number % num_fields) * field_size;

    unsigned int curval = ReadMMIO(reg);
    curval &= ~(field_mask << shift);
    curval |= value << shift;
    WriteMMIO(reg, curval);

    return 1;
}

unsigned int gpio_set(unsigned int pin_number,
	       	      unsigned int value) 
{ 
	return gpio_call(pin_number, value, GPSET0, 1, GPIO_MAX_PIN); 
}
unsigned int gpio_clear(unsigned int pin_number,
	       		unsigned int value) 
{ 
	return gpio_call(pin_number, value, GPCLR0, 1, GPIO_MAX_PIN); 
}
unsigned int gpio_pull(unsigned int pin_number,
	       	       unsigned int value) 
{
       	return gpio_call(pin_number, value, GPPUPPDN0, 2, GPIO_MAX_PIN); 
}
unsigned int gpio_function(unsigned int pin_number, 
			   unsigned int value) 
{ 
	return gpio_call(pin_number, value, GPFSEL0, 3, GPIO_MAX_PIN); 
}

void gpio_useAsAlt3(unsigned int pin_number) 
{
    gpio_pull(pin_number, Pull_None);
    gpio_function(pin_number, GPIO_FUNCTION_ALT3);
}

void gpio_useAsAlt5(unsigned int pin_number) 
{
    gpio_pull(pin_number, Pull_None);
    gpio_function(pin_number, GPIO_FUNCTION_ALT5);
}

void gpio_initOutputPinWithPullNone(unsigned int pin_number)
{	
    gpio_pull(pin_number, Pull_None);
    gpio_function(pin_number, GPIO_FUNCTION_OUT);
}

void gpio_setPinOutputBool(unsigned int pin_number, unsigned int onOrOff)
{
	if(onOrOff)
	{
		gpio_set(pin_number, 1);
	}
	else
	{
		gpio_clear(pin_number, 1);
	}
}


// UART

enum 
{
    AUX_BASE        = PERIPHERAL_BASE + 0x215000,
    AUX_IRQ	    = AUX_BASE,
    AUX_ENABLES     = AUX_BASE + 4,
    AUX_MU_IO_REG   = AUX_BASE + 64,
    AUX_MU_IER_REG  = AUX_BASE + 68,
    AUX_MU_IIR_REG  = AUX_BASE + 72,
    AUX_MU_LCR_REG  = AUX_BASE + 76,
    AUX_MU_MCR_REG  = AUX_BASE + 80,
    AUX_MU_LSR_REG  = AUX_BASE + 84,
    AUX_MU_MSR_REG  = AUX_BASE + 88,
    AUX_MU_SCRATCH  = AUX_BASE + 92,
    AUX_MU_CNTL_REG = AUX_BASE + 96,
    AUX_MU_STAT_REG = AUX_BASE + 100,
    AUX_MU_BAUD_REG = AUX_BASE + 104,
    AUX_UART_CLOCK  = 500000000,
    UART_MAX_QUEUE  = 16 * 1024
};

#define AUX_MU_BAUD(baud) ((AUX_UART_CLOCK/(baud*8))-1)

unsigned char uart_output_queue[UART_MAX_QUEUE];
unsigned int  uart_output_queue_write = 0;
unsigned int  uart_output_queue_read = 0;

void uart_init_c() 
{
    WriteMMIO(AUX_ENABLES, 1); //enable UART1
    WriteMMIO(AUX_MU_CNTL_REG, 0);
    WriteMMIO(AUX_MU_LCR_REG, 3); //8 bits
    WriteMMIO(AUX_MU_MCR_REG, 0);
    WriteMMIO(AUX_MU_IER_REG, 0);
    WriteMMIO(AUX_MU_IIR_REG, 0xC6); //disable interrupts
    WriteMMIO(AUX_MU_BAUD_REG, AUX_MU_BAUD(115200));
    gpio_useAsAlt5(14);
    gpio_useAsAlt5(15);
    WriteMMIO(AUX_MU_CNTL_REG, 3); //enable RX/TX
}

unsigned int uart_isOutputQueueEmpty()
{
	return uart_output_queue_read == uart_output_queue_write;
}

//sometime this or calling code needs some attention, these are checked all the time. perhaps it is good because of the different cores can check them at any time? 
unsigned int uart_isReadByteReady() 
{ 
	return ReadMMIO(AUX_MU_LSR_REG) & 0x01; 
}

//how many times per char does this need to be checked?
unsigned int uart_isWriteByteReady() 
{ 
	return ReadMMIO(AUX_MU_LSR_REG) & 0x20; 
}

unsigned char uart_readByte()
{
	while(!uart_isReadByteReady());
	return (unsigned char)ReadMMIO(AUX_MU_IO_REG);
}

void WriteByteToUART(unsigned char ch) 
{
    while (!uart_isWriteByteReady()); 
    WriteMMIO(AUX_MU_IO_REG, (unsigned int)ch);
}

void ReadAndWriteByte()
{
	WriteByteToUART(uart_readByte());
}

void uart_loadOutputFifo()
{
	while (!uart_isOutputQueueEmpty() && uart_isWriteByteReady())
	{
		WriteByteToUART(uart_output_queue[uart_output_queue_read]);
		uart_output_queue_read = (uart_output_queue_read + 1) & (UART_MAX_QUEUE -1);
	}
}

void WriteByteQueue(unsigned char ch)
{
	unsigned int next = (uart_output_queue_write + 1) & (UART_MAX_QUEUE -1);
	while(next == uart_output_queue_read) //change to if-statement?
		uart_loadOutputFifo();
	uart_output_queue[uart_output_queue_write] = ch;
	uart_output_queue_write = next;
}


void WriteTextUart(char *buffer) 
{
    while (*buffer) 
    {
      WriteByteToUART(*buffer++); 
    }
}

void uart_drainOutputQueue()
{
	while(!uart_isOutputQueueEmpty()) uart_loadOutputFifo();
}

void uart_update()
{
	uart_loadOutputFifo();
	if(uart_isReadByteReady())
	{
		unsigned char ch = uart_readByte();
		if(ch == '\r') WriteTextUart("\n"); //can I just loose this branch?
		else WriteByteQueue(ch);

	}
}


