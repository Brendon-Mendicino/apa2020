#include <stdio.h>
#include <stdlib.h>

#include "elementi.h"


static void memCheck( void *pionter)
{
	if( pionter == NULL) {
		fprintf( stderr, "ERROR: out of memory!\n");
		exit(EXIT_FAILURE);
	}
}

/* Creatore.  */
elemList_t *listInit( int max_elem)
{
	elemList_t *list;
	list = (elemList_t *)malloc(sizeof(elemList_t));
	memCheck( list);
	list->elem = (elemento_t *)malloc( max_elem * sizeof(elemento_t));
	memCheck( list->elem);
	list->max_elem = max_elem;
	list->n_elem = 0;
	return list;
}

/* Creatore.  */
Diag *diagInit( int n_diag)
{
	Diag *diag;
	diag = (Diag *)malloc( n_diag * sizeof(Diag));
	memCheck( diag);
	for( int i = 0; i < n_diag; i++) {
		diag[i] = (Diag)calloc( 1, sizeof(struct diag_t));
		memCheck( diag[i]);
		diag[i]->list = listInit( MAX_ELEM);
	}

	return diag;
}

/* Distruttore.  */
void listFree( elemList_t *list)
{
	free( list->elem);
	free( list);
}

/* Distruttore.  */
void diagFree( Diag *diag, int n_diag)
{
	for( int i = 0; i < n_diag; i++) {
		listFree( diag[i]->list);
		free( diag[i]);
	}
	free( diag);
}



/* Stampa la lista.  */
void listPrint( FILE *output, elemList_t *list)
{
	int i;
#	define elem(A)  (list->elem[(A)])
	for( i = 0; i < list->n_elem; i++) {
		fprintf( output, "%2d: ", i+1);
		fprintf( output, "%s\n%d %d %d %d %d %.3f %d\n",
				elem(i).nome, elem(i).type, elem(i).dir_entr, elem(i).dir_exit,
				elem(i).req_preced, elem(i).finale, elem(i).score, elem(i).diff);
	}
#	undef elem
}


/* Leggi da file.  */
void listReadFromFile( FILE *input, elemList_t *list)
{
	int i;
#	define elem(A)  (list->elem[(A)])
	for( i = 0; i < list->max_elem; i++) {
		fscanf( input, "%s%d%d%d%d%d%f%d", elem(i).nome, &elem(i).type,
				&elem(i).dir_entr, &elem(i).dir_exit, &elem(i).req_preced,
				&elem(i).finale, &elem(i).score, &elem(i).diff);
		list->n_elem++;
	}
#	undef elem
}

void printDiag( FILE *output, Diag diag)
{
	int i;
	float score = 0.0;
	fprintf( output, "DIFF: %d\n", diag->diff);
	for( i = 0; i < diag->list->n_elem; i++)
		score += diag->list->elem[i].score;
	fprintf( output, "SCORE: %.3f\n", score);
	for( i = 0; i < diag->list->n_elem; i++) {
		fprintf( output, "%s ", diag->list->elem[i].nome);
	}
	
	fprintf( output, "\n");
}





/* Controllo elemeto acrobatico.  */
int checkAcrobatico( elemList_t *list, int *index, int n_index)
{
	int i;
	int n_acr = 0;
	for( i = 0; i < n_index; i++) {
		if( list->elem[index[i]].type == acr_front || list->elem[index[i]].type == acr_back)
			n_acr++;
	}
	return n_acr;
}

/* Controllo se un tipo avanti e' presente.  */
int checkAcrobFront( elemList_t *list, int *index, int n_index)
{
	int n_front = 0;
	for( int i = 0; i < n_index; i++) {
		if( list->elem[index[i]].type == acr_front)
			n_front++;
	}
	return n_front;
}

/* Controllo se un tipo indietro e' presente.  */
int checkAcrobBack( elemList_t *list, int *index, int n_index)
{
	int n_back = 0;
	for( int i = 0; i < n_index; i++) {
		if( list->elem[index[i]].type == acr_back)
			n_back++;
	}
	return n_back;
}

/* Controllo due elementi consecutivi.  */
int checkAcrobConsecutive( elemList_t *list, int *index, int n_index)
{
	int flag = 0;
	int i;
	for( i = 0; i < n_index; i++) {
		if( list->elem[index[i]].type == acr_front || list->elem[index[i]].type == acr_back) {
			if( flag == 1)
				return 1;
			flag++;
		}
		else
			flag = 0;
	}
	return 0;
}

/* Controllo sul primo elemento, deve essre frontale.  */
int isFirstValid( elemList_t *list, int index)
{
	if( list->elem[index].dir_entr == 1)
		return 1;
	return 0;
}

/* Controllo sulla validita' dell'elemento in base al predecessore. */
int isNextValid( elemList_t *list, int last, int prev)
{
	if( list->elem[last].dir_entr == list->elem[prev].dir_exit)
		return 1;
	return 0;
}

/* Controllo se l'elemento puo essere suguito da altri.  */
int isLastElement( elemList_t *list, int index)
{
	if( list->elem[index].finale == 1)
		return 0;
	return 1;
}

/* Controlle se l'elemento ha bisogno di un elemento di precedenza.  */
int isReqDiPreced( elemList_t *list, int index)
{
	if( list->elem[index].req_preced == 1)
		return 1;
	return 0;
}


/* Controlla se un elemento con difficalta' '8' e' prente in una lista.  */
int isEightPresent( elemList_t *list, int *index, int n_index)
{
	for( int i = 0; i < n_index; i++) {
		if( list->elem[index[i]].diff >= 8)
			return 1;
	}
	return 0;
}

/* Prendi lo 'score' totale da una lista.  */
float getScore( elemList_t *list, int *index, int n_index)
{
	float score = 0.0;
	for( int i = 0; i < n_index; i++) {
		score += list->elem[index[i]].score;
	}
	return score;
}

/* Ritorna la differenza totale.  */
int listGetDiff( elemList_t *list, int *index, int n_index)
{
	int diff = 0;
	for( int i = 0; i < n_index; i++) {
		diff += list->elem[index[i]].diff;
	}
	return diff;
}



/* Copia una lista di elementi dentro una diagonale.  */
void diagCopy( Diag diag, int *index, int n_index, elemList_t *list)
{
	diag->diff = 0;
	for( int i = 0; i < n_index; i++) {
		diag->list->elem[i] = list->elem[index[i]];
		diag->diff += list->elem[index[i]].diff;
	}
	diag->list->n_elem = n_index;
}
