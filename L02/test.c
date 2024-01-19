#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main(void)
{
	char a = '\'';
	int b = 0;
	b = ispunct(a);
	printf( "%d\n", b);
	return 0;
}
