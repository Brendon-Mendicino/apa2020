#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol_t.h"


struct node_t {
	char *nome;
	int net;
	int value;
	struct node_t *next;
};


/* List linkata, non-ordinata.  */
struct symboltable_t {
	struct node_t *head;
	int n_items;
	int val_count; /* Tiene il conto dei valori delle chiavi.  */
	int max;
};




static void memCheck( void *p)
{
	if( p == NULL) {
		fprintf( stderr, "ERROR: out of memory!\n");
		exit( EXIT_FAILURE);
	}
}

static void freeR( struct node_t *x)
{
	if( x == NULL)
		return;
	freeR( x->next);
	free( x->nome);
	free( x);
}

static struct node_t *NewNode( char *nome, struct node_t *next)
{
	struct node_t *a;
	a = (struct node_t *)malloc(sizeof(struct node_t));
	memCheck( a);
	a->value = -1;
	a->net = -1;
	a->nome = strdup( nome);
	memCheck( a->nome);
	a->next = next;
	return a;
}



/* Costruttore, inizializza al massimo numero di elementi.  */
ST STinit( int max)
{
	ST st;
	st = (ST)malloc(sizeof(*st));
	memCheck(st);
	st->head = NULL;
	st->max = max;
	st->n_items = 0;
	st->val_count = 0;
	return st;
}

/* Distruttore.  */
void STfree( ST st)
{
	freeR( st->head);
	free( st);
}


/* Conta il numero di elementi nella tabella.  */
int STcount( ST st)
{
	return st->n_items;
}


/* Inserisci un nuovo elemento.  */
int STinsert( ST st, char *nome, int net)
{
	// 'value' e' il numero assegnato ad ogni 'nome'
	// Ricorda l'indece in un vettore
	st->head = NewNode( nome, st->head);
	st->head->value = st->val_count;
	st->head->net = net;
	st->val_count++;
	st->n_items++;
	return st->head->value;
}

/* Ritorna il valore associato ad un nome.  */
int STgetValue( ST st, char *nome)
{
	struct node_t *x;
	if( st == NULL)
		return -1;

	for( x = st->head; x != NULL; x = x->next) {
		if( strcmp( x->nome, nome) == 0)
			return x->value;
	}
	return -1;
}

/* Selezione l'elemeto r-esimo nella tabella.  */
int STselect( ST st, int r)
{
	struct node_t *x;
	x = st->head;

	if( r > st->n_items)
		return -1;
	for( int i = 0; i < r; i++)
		x = x->next;

	return x->value;
}

/* Cerca la chiave.  */
char *STsearchByValue( ST st, int value)
{
	struct node_t *x = st->head;

	while( x != NULL) {
		if( x->value == value)
			return x->nome;
		x = x->next;
	}
	return NULL;
}
