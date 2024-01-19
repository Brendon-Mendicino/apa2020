#include <stdio.h>

#define maxN 30

void ruota( int vettore[maxN], int N, int P, int dir);

int main( void)
{
	int vettore[maxN] = {0};
	int dir = 0, N = 0, P = 0;

	fprintf( stdout, "Dimensione del vettore (<= 30): ");
	scanf( "%d", &N);
	fprintf( stdout, "Inserisci il vettore: ");

	for( int i = 0; i < N; i++) {
		scanf( "%d", &vettore[i]);
	}

	putchar('[');
	for( int i = 0; i < N; i++) {
		fprintf( stdout, "%d ", vettore[i]);
	}
	putchar(']');
	putchar('\n');

	do {
		fprintf( stdout, "Inserire il numero di passi (< %d): ", N);
		do {
			scanf( "%d", &P);
		} while( !(P < N && P >= 0) );
		
		fprintf( stdout, "Inserire la direzine (-1 Dx, 1 Sx): ");
		scanf( "%d", &dir);

		ruota( vettore, N, P, dir);
		putchar('[');
		for( int i = 0; i < N; i++) {
			fprintf( stdout, "%d ", vettore[i]);
		}
		putchar(']');
		putchar('\n');
	} while( P != 0);

	return 0;
}


void ruota( int vettore[maxN], int N, int P, int dir)
{	
	int vettore_buff[maxN] = {0};

	for( int i = 0; i < N; i++) {
		vettore_buff[i] = vettore[i];
	}
	if( dir == 1) {
		int j = 0;
		for( int i = P; i < N; i++) {
			vettore[i] = vettore_buff[j];
			j++;
		}
		for( int i = 0; i < N -j; i++) {
			vettore[i] = vettore_buff[j + i];
		}
	}
	else {
		int j = 0;
		for( int i = P; i < N; i++) {
			vettore[j] =  vettore[i];
			j++;
		} 
		for( int i = 0; i < P; i++) {
			vettore[j] = vettore_buff[i];
			j++;
		}
	}
}
