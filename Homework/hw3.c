 _do_fork#include <stdio.h>
#include <unistd.h>

//1 - 16
//2 - A = 0, B = 2603, C = 2600, D = 2603
int main()
{
	int i;
	for (i=0; i<4; i++)
		fork();
	printf("child\n");
	return 0;
}

