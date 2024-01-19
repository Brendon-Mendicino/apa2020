#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "elementi.h"

#define DGB


typedef struct {
	int elem_acrob_per_diag;
	int elem_acrob_front;
	int elem_acrob_back;
	int diag_wiht_2_elem_cons;
} requisiti_t;



requisiti_t ReqNull( void)
{
	requisiti_t req;
	req.diag_wiht_2_elem_cons = 0;
	req.elem_acrob_per_diag = 0;
	req.elem_acrob_front = 0;
	req.elem_acrob_back = 0;
	return req;
}

#define N_OPTIONS 1

void printPrompt( void)
{
	//TODO: finire
	printf( "\n (1) ~> Usa bonta' come media della DP.\n");
	printf( ">>> ");
	fflush( stdout);
}

int readOption( int *opt)
{
	int temp;
	scanf( "%d", &temp);
	if( temp <= 0 || temp > N_OPTIONS)
		return 0;
	*opt = temp;
	return 1;
}


List_t loadFromFile( const char *file_name)
{
	List_t elementi;
	FILE *input;
	int n_elem;

	input = fopen( file_name, "r");
	if( input == NULL) {
		fprintf( stderr, "File: \"%s\" non trovato!\n", file_name);
		exit(EXIT_FAILURE);
	}

	fscanf( input, "%d", &n_elem);
	elementi = listInit( n_elem);
	listReadFromFile( input, elementi);
	return elementi;
}


void findSolAndPrint( List_t elementi)
{
	int term = 1;
	requisiti_t req = ReqNull();
	int DD, DP;
	int DF; // Difficolta' finale
	int diag1[MAX_ELEM], diag2[MAX_ELEM], diag3[MAX_ELEM];
	// Parametri che servono a decidere se una daigonale e' utilizzabile
	int d1 = 1, d2 = 1, d3 = 1;
	int diff1 = 0, diff2 = 0, diff3 = 0;

	// Ordino la lista con difficolta'  crescente
	listSort( elementi);

	printf( "Inserire la DD e la DP: ");
	scanf( "%d%d", &DD, &DP);

	// Questa e' la difficolta' che non si puo superare per ogni diagonale
	DP = DP / 3;
	DF = ( DP > DD)? DD: DP;


	int i, j, k;
	for( i = 0; term; i++) {
		// Decidono se il seguente passo e' relizzabile
		int step1 = 1;
		int step2 = 1;
		int step3 = 1;
		for( k = 0; k < elementi->n_elem; k++) {
			diag1[i] = k;
			diag2[i] = k;
			diag3[i] = k;
			if( d1 && step1 && listGetDiff( elementi, diag1, i) <= DF) {
				if( i == 0 && isFirstValid( elementi, k)) {
					diag1[i] = k;
					step1 = 0;
				}
			}
			if( d2 && step2 && listGetDiff( elementi, diag2, i) <= DF) {
				if( i == 0 && isFirstValid( elementi, k)) {
					diag2[i] = k;
					step2 = 0;
				}
			}
			if( d3 && step3 && listGetDiff( elementi, diag3, i) <= DF) {
				if( i == 0 && isFirstValid( elementi, k)) {
					diag3[i] = k;
					step3 = 0;
				}
			}
			if( step1 == 0 && step2 == 0 && step3 == 0)
				break;
		}
	}
}


int main( int argc, char **argv)
{
	List_t elementi;
	int loop = 1;
	int opt;


	if( argc != 2) {
		fprintf( stderr, "Uso non corretto!\n");
		exit(EXIT_FAILURE);
	}
	elementi = loadFromFile( argv[1]);

#	ifdef DGB
	listPrint( stdout, elementi);
#	endif

	while( loop) {
		printPrompt();
		if( !readOption( &opt)) {
			fprintf( stderr, "Opzioine non valida!\n");
			continue;
		}
		if( opt == 1)
		{
			findSolAndPrint( elementi);
		}
	}


	listFree( elementi);
	return 0;
}
