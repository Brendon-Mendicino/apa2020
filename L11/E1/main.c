#include <stdio.h>
#include <stdlib.h>

#include "elementi.h"


#define N_DIAG 3

#define DBG 1


typedef struct {
	int elem[MAX_ELEM];
	int n_elem;
	int diff_tot;
} my_list;

typedef struct {
	my_list *list;
	int n_list;
	int max_list;
} index_list_t;	/* Lista delle diagonali possibili.  */





static void memCheck( void *a)
{
	if( a == NULL) {
		fprintf( stderr, "ERROR: out of memory!\n");
		exit(EXIT_FAILURE);
	}
}



index_list_t *indexInit( void)
{
	index_list_t *a;
	a = (index_list_t *)malloc(sizeof(*a));
	memCheck( a);
	a->list = (my_list *)calloc( 1, sizeof(my_list));
	memCheck( a->list);

	a->n_list = 0;
	a->max_list = 1;
	return a;
}

void indexFree( index_list_t *index_list)
{
	free( index_list->list);
	free( index_list);
}

void listAdd( int *index, int n_index, int diff, index_list_t *index_list)
{
	index_list->n_list++;
	if( index_list->n_list > index_list->max_list) {
		index_list->max_list = index_list->max_list * 2;
		index_list->list = (my_list *)realloc( index_list->list, index_list->max_list * sizeof(my_list));
		memCheck( index_list->list);
	}
	for( int i = 0; i < n_index; i++) {
		index_list->list[index_list->n_list-1].elem[i] = index[i];
	}
	index_list->list[index_list->n_list-1].n_elem = n_index;
	index_list->list[index_list->n_list-1].diff_tot = diff;
}





elemList_t *readFromFileAndInit( char *file_name)
{
	elemList_t *list;
	FILE *input;
	int n_elem;

	input = fopen( file_name, "r");
	if( input == NULL) {
		fprintf( stderr, "File: \"%s\" not found!\n", file_name);
		exit(EXIT_FAILURE);
	}

	fscanf( input, "%d", &n_elem);
	list = listInit( n_elem);
	listReadFromFile( input, list);

	fclose( input);
	return list;
}






int checkForAcrobFrontAndBack( elemList_t *list, int *sol, index_list_t *index_list)
{
	int i;
	int front_flag = 0;
	int back_flag = 0;

	for( i = 0; i < N_DIAG; i++) {
		if( checkAcrobFront( list, index_list->list[sol[i]].elem, index_list->list[sol[i]].n_elem)) {
			front_flag = 1;
			break;
		}
	}
	for( i = 0; i < N_DIAG; i++) {
		if( checkAcrobBack( list, index_list->list[sol[i]].elem, index_list->list[sol[i]].n_elem)) {
			back_flag = 1;
			break;
		}
	}
	if( back_flag && front_flag)
		return 1;
	return 0;
}

int checkForConsecutiveAcrob( elemList_t *list, int *sol, index_list_t *index_list)
{
	int i;
	for( i = 0; i < N_DIAG; i++) {
		if( checkAcrobConsecutive( list, index_list->list[sol[i]].elem, index_list->list[sol[i]].n_elem))
			return 1;
	}
	return 0;
}


void genera_diagonali( int pos, int *sol, int *curr_diff, int DD, elemList_t *list, index_list_t *possible_diag)
{
	int i = 0;
	if( pos > MAX_ELEM)
		return;

	if( *curr_diff <= DD  &&  *curr_diff > 0) {
		if( checkAcrobatico( list, sol, pos)){
			listAdd( sol, pos, *curr_diff, possible_diag);
		}
	}
	/* Se l'ultimo elemento e' un elemeto finale ritorno alla ricorsione precedente.  */
	if( pos > 0 && !isLastElement( list, sol[pos-1]))
		return;

	for( i = 0; i < list->n_elem; i++) {
		/* i-esimo elemento  */
		sol[pos] = i;
		if( pos == 0) {
			if( !isFirstValid( list, sol[pos]))
				continue;
			if( isReqDiPreced( list, sol[pos]))
				continue;
			*curr_diff += list->elem[sol[pos]].diff;
			genera_diagonali( pos+1, sol, curr_diff, DD, list, possible_diag);
			*curr_diff -= list->elem[sol[pos]].diff;
		}
		else {
			if( !isNextValid( list, sol[pos], sol[pos-1]))
				continue;
			*curr_diff += list->elem[sol[pos]].diff;
			genera_diagonali( pos+1, sol, curr_diff, DD, list, possible_diag);
			*curr_diff -= list->elem[sol[pos]].diff;
		}
	}
}

void copyToDiag( elemList_t *list, Diag *diag, int *sol, index_list_t *index_list)
{
	int i;
	for( i = 0; i < N_DIAG; i++) {
		diagCopy( diag[i], index_list->list[sol[i]].elem, index_list->list[sol[i]].n_elem, list);
	}
}

int totDiff( elemList_t *list, int *sol, index_list_t *index_list)
{
	int diff = 0;
	for( int i = 0; i < N_DIAG; i++) {
		diff += listGetDiff( list, index_list->list[sol[i]].elem, index_list->list[sol[i]].n_elem);
	}
	return diff;
}

void trovaDiagonaliValide( int pos, float *max_score, int *sol,
						   elemList_t *list, Diag *diag, int DP, index_list_t *possible_diag)
{
	static int curr_diff = 0;
	static float curr_score = 0.0;
	float moltiplicatore = 1.0;

	if( pos >= 3) {
		if( checkForAcrobFrontAndBack( list, sol, possible_diag)  &&
			checkForConsecutiveAcrob( list, sol, possible_diag)   &&
			totDiff( list, sol, possible_diag) <= DP) {
			if( curr_score > *max_score) {
				*max_score = curr_score;
				copyToDiag( list, diag, sol, possible_diag);
			}
		}
		return;
	}

	for( int i = 0; i < possible_diag->n_list; i++) {
		/* Interi della lista 'possible_diag->list'.  */
		sol[pos] = i;
		curr_diff += possible_diag->list[sol[pos]].diff_tot;			
		if( pos < N_DIAG-1) {
			curr_score += getScore( list, possible_diag->list[sol[pos]].elem, possible_diag->list[sol[pos]].n_elem);
			trovaDiagonaliValide( pos+1, max_score, sol, list, diag, DP, possible_diag);
			curr_score -= getScore( list, possible_diag->list[sol[pos]].elem, possible_diag->list[sol[pos]].n_elem);
		}
		else {
			if( isEightPresent( list, possible_diag->list[sol[pos]].elem, possible_diag->list[sol[pos]].n_elem))
				moltiplicatore = 1.5;

			curr_score += moltiplicatore * getScore(
					list, possible_diag->list[sol[pos]].elem, possible_diag->list[sol[pos]].n_elem);

			trovaDiagonaliValide( pos+1, max_score, sol, list, diag, DP, possible_diag);

			curr_score -= moltiplicatore * getScore(
					list, possible_diag->list[sol[pos]].elem, possible_diag->list[sol[pos]].n_elem);
			moltiplicatore = 1.0;
		}
		curr_diff -= possible_diag->list[sol[pos]].diff_tot;			
	}
}

void fillDiag( elemList_t *list, Diag *diag, int DD, int DP)
{
	index_list_t *possible_diag;
	int curr_diff = 0;
	float max_score = 0.0;
	int *sol;
	possible_diag = indexInit();
	sol = (int *)calloc( MAX_ELEM, sizeof(int));

	genera_diagonali( 0, sol, &curr_diff, DD, list, possible_diag);
#	ifdef DBG
	for( int i = 0; i < possible_diag->n_list; i++) {
		for( int j = 0; j < possible_diag->list[i].n_elem; j++)
			printf( "%d ", possible_diag->list[i].elem[j]);
		printf( "\n");
	}
#	endif

	sol = (int *)realloc( sol, possible_diag->n_list * sizeof(int));
	trovaDiagonaliValide( 0, &max_score, sol, list, diag, DP, possible_diag);
	
	printf( "MAX_SCORE: %.3f\n", max_score);
	for( int i = 0; i < N_DIAG; i++) {
		printf( "DIAGONALE #%d:\n", i+1);
		printDiag( stdout, diag[i]);
	}


	indexFree( possible_diag);
	free( sol);
}




int main( void)
{
	elemList_t *list;
	Diag *diag;
	int DD, DP;

	list = readFromFileAndInit( "elementi.txt");
	listPrint( stdout, list);

	diag = diagInit( N_DIAG);

	printf( "Inserire la difficolta' di una diagonale: ");
	scanf( "%d", &DD);
	printf( "Inserire la difficolta' del programma: ");
	scanf( "%d", &DP);

	fillDiag( list, diag, DD, DP);

	listFree( list);
	diagFree( diag, N_DIAG);
	return 0;
}
