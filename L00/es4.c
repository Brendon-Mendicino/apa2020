#include <stdio.h>
#include <stdlib.h>

#define MAX 20

typedef struct {
	int x;
	int y;
	int sum;
	int dim;
} Big_matr;

int main( void)
{
	FILE* input;
	Big_matr big_matr = {0};
	char nome_file[21] = {0};
	int matrice[MAX][MAX] = {{0}};
	int righe = 0, colonne = 0;
	int dim = 0, buffer_sum = 0;

	printf( "Inserire il nome del file: ");
	scanf( "%s", nome_file);

	if( (input = fopen( nome_file, "r") ) == NULL) {
		fprintf( stderr, "File inesistente!\n");
		exit(1);
	}

	fscanf( input, "%d%d", &righe, &colonne);
	for( int i = 0; i < righe; i++) {
		for( int j = 0; j < colonne; j++) {
			fscanf( input, "%d", &matrice[i][j]);
		}
	}
	for( int i = 0; i < righe; i++) {
		putchar('[');
		for( int j = 0; j < colonne; j++) {
			printf( "%d ", matrice[i][j]);
		}
		putchar(']');
		putchar('\n');
	}
	putchar('\n');

	do {
		int dim_buffer = 0;
		printf( "Inserire la dimensione (>0, <=%d): ", (righe < colonne) ? righe: colonne );
		scanf( "%d", &dim_buffer);
		if( dim_buffer > 0 && dim_buffer < righe && dim_buffer < colonne)
			dim = dim_buffer;
		else break;

		for(int x = 0; x <= righe -dim; x++) {
			for( int y = 0; y <= colonne -dim; y++) {

				printf( "coord (%d, %d)\n", x, y);
				for( int i = 0; i < dim; i++) {
					putchar('[');
					for( int j = 0; j < dim; j++) {
						printf( "%d ", matrice[x+i][y+j]);
						buffer_sum += matrice[x+i][y+j];
					}
					putchar(']');
					putchar('\n');
				}
				putchar('\n');
				if( buffer_sum > big_matr.sum) {
					big_matr.sum = buffer_sum;
					big_matr.x = x;
					big_matr.y = y;
					big_matr.dim = dim;
				}
				buffer_sum = 0;
			}
		}

	} while( dim > 1 && dim < righe && dim < colonne);

	printf( "Somma interna piu' grande!\n");
	for( int i = 0; i < big_matr.dim; i++) {
		putchar('[');
		for( int j = 0; j < big_matr.dim; j++) {
			printf( "%d ", matrice[big_matr.x + i][big_matr.y + j]);
		}
		putchar(']');
		putchar('\n');
	}


	fclose( input);
	return 0;
}
