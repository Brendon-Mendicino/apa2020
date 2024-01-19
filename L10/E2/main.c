/* Brendon Mendicino, 271784 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N_PIETRE 4



typedef struct {
	int z, r, s, t;
} pietre_t;


int fS( int *****matr, int z, int r, int t, int s);
int fT( int *****matr, int z, int r, int t, int s);
int fR( int *****matr, int z, int r, int t, int s);
int fZ( int *****matr, int z, int r, int t, int s);


void pointer_check( void *x)
{
	if( x == NULL) {
		fprintf( stderr, "ERROR: out of memory!\n");
		exit(EXIT_FAILURE);
	}
}


int *****matriceInit( pietre_t *p)
{
	/* Squeza delle pietre.  */
	/* z, r, t, s;  */
	int *****matr;
	int a, i, j, k;

	matr = (int *****)malloc( 4 * sizeof(int ****));
	pointer_check( matr);
	for( a = 0; a < 4; a++) {
		matr[a] = (int ****)malloc( (p->z+1) * sizeof(int ***));
		pointer_check( matr);

		for( i = 0; i <= p->z; i++) {
			matr[a][i] = (int ***)malloc( (p->r+1) * sizeof(int **));
			pointer_check( matr);

			for( j = 0; j <= p->r; j++) {
				matr[a][i][j] = (int **)malloc( (p->t+1) * sizeof(int *));
				pointer_check( matr);
				
				for( k = 0; k <= p->t; k++) {
					matr[a][i][j][k] = (int *)calloc( p->s+1, sizeof(int));
					pointer_check( matr);
				}
			}
		}
	}
	return matr;
}

void matrFree( pietre_t *p, int *****matr)
{
	/* Squeza delle pietre.  */
	/* z, r, t, s;  */
	int a, i, j, k;
	for( a = 0; a < 4; a++) {
		for( i = 0; i <= p->z; i++) {
			for( j = 0; j <= p->r; j++) {
				for( k = 0; k <= p->t; k++) {
					free( matr[a][i][j][k]);
				}
				free( matr[a][i][j]);
			}
			free( matr[a][i]);
		}
		free( matr[a]);
	}
	free( matr);
}

void pietrePrint( FILE *output, pietre_t *p)
{
	/* Squeza delle pietre.  */
	/* z, r, t, s;  */
	fprintf( output, "Z:%d R:%d T:%d S:%d\n", p->z, p->r, p->t, p->s);
}

int max( int a, int b)
{
	return ((a >= b)? a: b);
}


int warapperRicorsione( int *****matr, pietre_t *p)
{
	int a, b;
	a = max( fZ( matr, p->z, p->r, p->t, p->s), fR( matr, p->z, p->r, p->t, p->s));
	b = max( fT( matr, p->z, p->r, p->t, p->s), fS( matr, p->z, p->r, p->t, p->s));
	return ((a >= b)? a: b);
}


void risolvi( pietre_t *pietre)
{
	int *****matr;
	int max_len = 0;

	matr = matriceInit( pietre);

	max_len = warapperRicorsione( matr, pietre);

	printf( "Lunghezza max: %d\n\n", max_len);

	matrFree( pietre, matr);
}


int main( int argc, char **argv)
{
	pietre_t *pietre;
	pietre = (pietre_t *)calloc( 1, sizeof(pietre_t));
	pointer_check( pietre);

	
	if( argc == 2) {
		FILE *in;
		int iterations;

		in = fopen( argv[1], "r");
		if( in == NULL) {
			fprintf( stderr, "File: \"%s\" non trovato!\n", argv[1]);
			exit(EXIT_FAILURE);
		}

		fscanf( in, "%d", &iterations);
		for( int i = 0; i < iterations; i++) {
			/* Leggo le pietre.  */
			fscanf( in, "%d%d%d%d", &pietre->z, &pietre->r, &pietre->t, &pietre->s);
			pietrePrint( stdout, pietre);
			
			risolvi( pietre);
		}

		fclose( in);
	}
	else {
		fprintf( stderr, "Argomento non valido!\n");
		exit(EXIT_FAILURE);
	}

	free( pietre);
	return 0;
}



/* Squeza delle pietre.  */
/* z, r, t, s;  */
/* A:
   0 = z,
   1 = r,
   2 = t,
   3 = s
   */
# define matr_scelta(A)   ( matr[(A)][z][r][t][s])

int fS( int *****matr, int z, int r, int t, int s)
{
	if( matr_scelta(3) != 0)
		return matr_scelta(3);
	
	if( s <= 0)
		return 0;
	/* +1 dato dalla pietra di questa ricorsione.  */
	matr_scelta(3) = 1 + max( fS( matr, z, r, t, s-1), fT(matr, z, r, t, s-1));
	return matr_scelta(3);
}

int fT( int *****matr, int z, int r, int t, int s)
{
	if( matr_scelta(2) != 0)
		return matr_scelta(2);
	
	if( t <= 0)
		return 0;
	/* +1 dato dalla pietra di questa ricorsione.  */
	matr_scelta(2) = 1 + max( fZ( matr, z, r, t-1, s), fR(matr, z, r, t-1, s));
	return matr_scelta(2);
}

int fR( int *****matr, int z, int r, int t, int s)
{
	if( matr_scelta(1) != 0)
		return matr_scelta(1);
	
	if( r <= 0)
		return 0;
	/* +1 dato dalla pietra di questa ricorsione.  */
	matr_scelta(1) = 1 + max( fT( matr, z, r-1, t, s), fS(matr, z, r-1, t, s));
	return matr_scelta(1);
}

int fZ( int *****matr, int z, int r, int t, int s)
{
	if( matr_scelta(0) != 0)
		return matr_scelta(0);
	
	if( z <= 0)
		return 0;
	/* +1 dato dalla pietra di questa ricorsione.  */
	matr_scelta(0) = 1 + max( fZ( matr, z-1, r, t, s), fR(matr, z-1, r, t, s));
	return matr_scelta(0);
}

#undef matr_scelta
