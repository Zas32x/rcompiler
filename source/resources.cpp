#include "resources.h"

int resInt(const char *str)
{
	int val=0;
	for(int i=0;str[i]!=NULL;i++)
	{
		switch (str[i])
		{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				{
					val*=10;
					val+=str[i]-'0';
				}
				break;
			default:
				return -1;
		}
	}
	return val;
}
