#include <stdio.h>
#include <stdlib.h>

int main( void)
{
	FILE *a;
	int b = 0;
	int c;
	a = fopen( "fil", "r");

	while( (c = fgetc(a)) != EOF)
		if( c == '\n')
			b++;

	printf( "%d\n", b);
	return 0;
}
