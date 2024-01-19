#ifndef INVARRAY_H_DEFINED
#define INVARRAY_H_DEFINED

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "inv.h"

/* ADT di prima classe collezione di oggetti di inventario */
typedef struct invArray_s *invArray_t;

/* creatore */
invArray_t invArray_init( void);
/* distruttore */
void invArray_free(invArray_t invArray);

/* lettura da file */
void invArray_read(FILE *fp, invArray_t invArray);
/* scrittura su file */
void invArray_print(FILE *fp, invArray_t invArray);
/* stampa un unico oggetto selezionato da indice (nel vettore) */
void invArray_printByIndex(FILE *fp, invArray_t invArray, int index);
/* ritorna puntatore a oggetto selezionato da indice (nel vettore) */
inv_t *invArray_getByIndex(invArray_t invArray, int index);
/* ritorna indice (nel vettore) a oggetto selezionato da nome */
int invArray_searchByName(invArray_t invArray, char *name);

/* Si possono aggiungere altre funzioni se ritenute necessarie */

/* controlla se l'indice e' valido */
int invArray_checkIndex( invArray_t invArray, int index);

#endif
