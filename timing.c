void wait_msec(unsigned int n)
{
	register unsigned long freq, time, r;

	//get current counter frequency
	asm volatile ("mrs %0, cntfrq_el0" : "=r"(freq));
	//read current counter
	asm volatile ("mrs %0, cntpct_el0" : "=r"(time));
	//calculate expire value for counter
	time += ((freq/1000)*n)/1000;
	do
	{
		asm volatile ("mrs %0, cntpct_el0" : "=r"(r));
	}while(r<time);
}

unsigned long cntpct_el0() {
	register unsigned long count;
	asm volatile ("mrs %0, cntpct_el0" : "=r"(count));
	return count;
}
