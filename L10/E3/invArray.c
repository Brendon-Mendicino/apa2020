#include "invArray.h"
#include <stdlib.h>


struct invArray_s {
	inv_t *inv;
	int n_invs;
};


/* creatore */
invArray_t invArray_init( void)
{
	invArray_t invArray;
	invArray = (invArray_t)malloc(sizeof(*invArray));
	if( invArray == NULL) {
		fprintf( stderr, "ERROR: out of memory!\n");
		exit(EXIT_FAILURE);
	}
	invArray->n_invs = 0;

	return invArray;
}
/* distruttore */
void invArray_free(invArray_t invArray)
{
	free( invArray->inv);
	free( invArray);
}




/* lettura da file */
void invArray_read(FILE *fp, invArray_t invArray)
{
	fscanf( fp, "%d", &invArray->n_invs);

	invArray->inv = (inv_t *)malloc(invArray->n_invs * sizeof(inv_t));
	for( int i = 0; i < invArray->n_invs; i++)
		inv_read( fp, &invArray->inv[i]);
}
/* scrittura su file */
void invArray_print(FILE *fp, invArray_t invArray)
{
	for( int i = 0; i < invArray->n_invs; i++) {
		fprintf( fp, "%2d) ", i+1);
		inv_print( fp, &invArray->inv[i]);
		fprintf( fp, "\n");
	}
}


/* stampa un unico oggetto selezionato da indice (nel vettore) */
void invArray_printByIndex(FILE *fp, invArray_t invArray, int index)
{
	if( !( index >= 0  &&  index < invArray->n_invs)) {
		fprintf( stderr, "No match found!\n");
		return;
	}
	inv_print( fp, &invArray->inv[index]);
	fprintf( fp, "\n");
}


/* ritorna puntatore a oggetto selezionato da indice (nel vettore) */
inv_t *invArray_getByIndex(invArray_t invArray, int index)
{
	if( !( index >= 0  &&  index < invArray->n_invs))
		return NULL;
	return (&invArray->inv[index]);
}



/* ritorna indice (nel vettore) a oggetto selezionato da nome */
int invArray_searchByName(invArray_t invArray, char *name)
{
	for( int i = 0; i < invArray->n_invs; i++) {
		if( strcmp( invArray->inv[i].nome, name) == 0)
			return i;
	}
	return -1;
}



/* controlla se l'indice e' valido */
int invArray_checkIndex( invArray_t invArray, int index)
{
	if( !( index >= 0  &&  index < invArray->n_invs)) {
		fprintf( stderr, "Invalid index!\n");
		return 0;
	}
	return 1;
}
