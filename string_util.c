
char* parse_ulong(int num, int base_)
{
	if(base_ < 2)
	{
		base_ = 2;
	}
	else if(base_ > 16)
	{
		base_ = 16;
	}
    	static char result[32] = {0}; //this will be static (local persist) until I have "malloc". unless I keep it like this
    	char *ptr1 = &result[0], tmp_char;
    	int tmp_value;
    	int base = base_;

	//TODO Oscar: add support for the character '.' to be able to print floats
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

void Concat(char* first, char* second)
{
	int countFirst = 0;
	while(*(first + countFirst) != '\0')
	{
		++countFirst;
	}

	int countSecond = 0;
	while(*(second + countSecond) != '\0')
	{
		*(first + countFirst) = *(second + countSecond);
		++countFirst;
		++countSecond;	
	}

	*(first + countFirst) = '\0';
}
