
#include <stdio.h>

#define N 19



int main(void)
{
	int A[N] = {0};
	int B[N] = {0};
	int counter = 0, max = 0;
	int flag = 0;

	for (int i = 0; i < N; i++) {
		scanf("%d", &A[i] );
	}
	putchar( '[');
	for (int i = 0; i < N; i++) {
		printf("%d ", A[i]);
	}
	putchar( ']');
	putchar( '\n');

	for (int i = 0; i < N; i++)
	{
		if (A[i] != 0)
			counter++;
		else
			counter = 0;

		if (counter > max)
			max = counter;
	}


	for (int i = 0; i < N; i++) {
		if (A[i] != 0) {
			B[flag] = A[i];
			flag++;
		}
		else { 
			flag = 0;
		}
		
		if (flag == max) {
			putchar('[');
			for(int j = 0; j < flag; j++) {
				printf( "%d ", B[j]);
			}
			putchar(']');
			putchar('\n');
		}
	}

	putchar('\n');
	return 0;
}
