#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void part( int pos, int *val, int *sol, int k, int start)
{
	if( start >= k) {
		for( int i = 0; i < pos; i++)
			printf( "%d ", sol[i]);
		printf( "\n");
		return;
	}

	for( int i = start; i < k; i++) {
		sol[pos] = val[i];
		part( pos+1, val, sol, k, i+1);
	}
	part( pos, val, sol, k, k);
}

int main(void)
{
	int val[4] = {1, 2, 3, 4};
	int sol[4];

	part( 0, val, sol, 4, 0);

	system("fortune | cowsay");
	return 0;
}
