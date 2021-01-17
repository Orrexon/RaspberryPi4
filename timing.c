unsigned long freq = 0;
unsigned long systemCounter = 0;

void wait_millisec(unsigned int n)
{
	register unsigned long time, r;

	//read current counter
	asm volatile ("mrs %0, cntpct_el0" : "=r"(time));
	//calculate expire value for counter
	time += ((freq/1000)*n);
	do
	{
		asm volatile ("mrs %0, cntpct_el0" : "=r"(r));
	}while(r<time);
}

unsigned long millisec_count()
{
	register unsigned long count;
	asm volatile ("mrs %0, cntpct_el0" : "=r"(count));
	return count / (freq/1000);
}
unsigned long millisec_count_delta(unsigned long oldCount)
{
	register unsigned long count;
	asm volatile ("mrs %0, cntpct_el0" : "=r"(count));
	return (count - oldCount) / (freq/1000);
}

void timing_init()
{
	if(!freq)
	{
		//get current counter frequency
		asm volatile ("mrs %0, cntfrq_el0" : "=r"(freq));
	}
	asm volatile ("mrs %0, cntpct_el0" : "=r"(systemCounter));
}
