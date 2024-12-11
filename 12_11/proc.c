#include <stdio.h>                                                                                                                         
#include <string.h>
int main(int argc, char *argv[])
{
	if(argc > 1)
	{
		if(strcmp(argv[1], "-a") == 0)
		{
			 printf("you used -a option...\n");
		}
		else if(strcmp(argv[1], "-b") == 0)
		{
			printf("you used -b option...\n");
		}
		else
		{
			printf("you used unrecognizable option...\n");
		}
	}
	else
	{
		printf("you did not use any option...\n");
	}
	return 0;
}
