/* Brendon Mendicino, 271784 */

#include <stdio.h>

typedef enum {false, true} bool;

int gcd( int, int);

int main( void)
{
	int a = 0, b = 0;
	bool loop = true;

	printf( "Colcolo del gcd (greatest common divisor)!\n");
	do {
		printf( "Inserire due numeri: ");
		if( scanf( "%d%d", &a, &b) != 2)
			loop = false;
		printf( "gcd = %d\n\n", gcd( a, b));
	} while(loop);

	return 0;
}


int gcd( int a, int b)
{
	int temp = 0;

	/* caso a < b */
	if( a < b) return gcd( b, a);

	if( a == b) return a;

	if( (a%2) == 0 && (b%2) == 0) {
		temp = 2 * gcd( a/2, b/2);
	}
	else if( (a%2) == 1 && (b%2) == 0) {
		temp = gcd( a, b/2);
	}
	else if( (a%2) == 0 && (b%2) == 1) {
		temp = gcd( a/2, b);
	}
	else /* if( (a%2) == 1 && (b%2) == 1) */ {
		/* se a e' un multiplo di b, con questa operzione arriveremo
		   sicuramente a b */
		temp = gcd( (a-b)/2, b);
	}

	return temp;
}
