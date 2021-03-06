#include "ioc.h"

// The buffer must be 16-byte aligned as only the upper 28 bits of the address can be passed via the mailbox
volatile unsigned int __attribute__((aligned(16))) mbox[36];

enum
{
	VIDEOCORE_MBOX = (PERIPHERAL_BASE + 0x0000B880),
	MBOX_READ      = (VIDEOCORE_MBOX + 0x0),
	MBOX_POLL      = (VIDEOCORE_MBOX + 0x10),
	MBOX_SENDER    = (VIDEOCORE_MBOX + 0x14),
	MBOX_STATUS    = (VIDEOCORE_MBOX + 0x18),
	MBOX_CONFIG    = (VIDEOCORE_MBOX + 0x1C),
	MBOX_WRITE     = (VIDEOCORE_MBOX + 0x20),
	MBOX_RESPONSE  = 0x80000000,
	MBOX_FULL      = 0x80000000,
	MBOX_EMPTY     = 0x40000000
};

unsigned int MboxVcCall(unsigned char ch)
{
	//28 bit address MSB and 4 bit value LSB
	unsigned int reg = ((unsigned int)((long) &mbox) &~ 0XF) | (ch & 0xF);

	//wait until we can write
	while (ReadMMIO(MBOX_STATUS) & MBOX_FULL);

	// Write the address of our buffer to the mailbox with the channel appended
	WriteMMIO(MBOX_WRITE, reg);

	while(1)
	{
		//is there a reply?? 
		while(ReadMMIO(MBOX_STATUS) & MBOX_EMPTY);

		//is it a reply to our message? 
		if(reg == ReadMMIO(MBOX_READ))
		{
			return mbox[1] == MBOX_RESPONSE;// Is it successful?
		}
	}
	return 0;
}

