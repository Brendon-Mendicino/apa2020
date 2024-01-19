/* Brendon Mendicino, 271784 */

#include <stdio.h>
#include <stdlib.h>


void apri_file( FILE **in, char *src);
int **malloc2d_and_save( FILE *in, int *nr, int *nc);
void stampa_matrice( int **matr, int nr, int nc);

void separa( int **matr, int nr, int nc, int *v_nero, int *v_bianco);


int main(void)
{
	FILE *in;
	int nr, nc;

	apri_file( &in, "matrice.txt");
	int **matrice = malloc2d_and_save( in, &nr, &nc);

	stampa_matrice( matrice, nr, nc);

	int *v_nero, *v_bianco;
	/* Alloco la memoria per i due vettri */
	/* Aggiungendo il '+1' i conti tornano anche per le matrice dispari */
	/* Infatti se la matrice e' dispari il 'v_nero' dovra' avere +1
	   caselle rispetto al 'v_bianco' */
	if( (v_nero = (int *)malloc((nr*nc+1)/2 * sizeof(int))) == NULL) {
		fprintf( stderr, "ERROR: out of memory!\n");
		exit(EXIT_FAILURE);
	}
	if( (v_bianco = (int *)malloc(nr*nc/2 * sizeof(int))) == NULL) {
		fprintf( stderr, "ERROR: out of memory!\n");
		exit(EXIT_FAILURE);
	}
	
	separa( matrice, nr, nc, v_nero, v_bianco);

	fclose(in);
	free(v_nero);
	free(v_bianco);
	for(int i = 0; i < nr; i++)
		free(matrice[i]);
	free(matrice);
	return 0;
}


void apri_file( FILE **in, char *src)
{
	if( (*in = fopen( src, "r")) == NULL) {
		fprintf( stderr, "File: \"%s\" non trovato!\n", src);
		exit(1);
	}
}

int **malloc2d_and_save( FILE *in, int *nr, int *nc)
{
	fscanf( in, "%d%d", nr, nc);

	int **matr;
	if( (matr = (int **)malloc(*nr * sizeof(int *)) ) == NULL ) {
		fprintf( stderr, "ERROR: out of memory!\n");
		exit(EXIT_FAILURE);
	}

	for( int i = 0; i < *nr; i++) {
		if( (matr[i] = (int *)malloc(*nc * sizeof(int)) ) == NULL ) {
			fprintf( stderr, "Can't allocate mem!\n");
			exit(EXIT_FAILURE);
		}
		/* Leggi File */
		for( int j = 0; j < *nc; j++)
			fscanf( in, "%d", &matr[i][j]);
	}

	return matr;
}

void stampa_matrice( int **matr, int nr, int nc)
{
	for( int i = 0; i < nr; i++)
		for( int j = 0; j < nc; j++)
			printf( "%d%c", matr[i][j], (j==nc-1)? '\n': ' ');
}


void separa( int **matr, int nr, int nc, int *v_nero, int *v_bianco)
{
	int n = 0, b = 0;
	int signal = 0;

	for( int i = 0; i < nr; i++) {
		if( i%2 == 0) signal = 0;
		else signal = 1;

		for( int j = 0; j < nc; j++) {
			if( signal == 0) {
				v_nero[n++] = matr[i][j];
				signal = 1;
			}
			else {
				v_bianco[b++] = matr[i][j];
				signal = 0;
			}
		}
	}

	printf( "Quadrati neri:\n");
	for( int i = 0; i < n; i++)
		printf( "%d%c", v_nero[i], (i==n-1)? '\n': ' ');

	printf( "\nQuadrati bianchi:\n");
	for( int i = 0; i < b; i++)
		printf( "%d%c", v_bianco[i], (i==b-1)? '\n': ' ');

}
