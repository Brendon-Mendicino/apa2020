#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mytime.h"
#include "quotazione.h"
#include "titolo.h"


typedef struct {
	mytime_t time;
	int value;
	int number;
	int is_read;
} transazione_t;


struct titolo_t {
	char code[MAX_CODE];
	Qbst quot_coll;
};


typedef struct node *link;

struct node {
	Titolo titolo;
	link next;
};


struct tlist_t {
	link head;
	int n_titoli;
};





static void memCheck( void *p)
{
	if( p == NULL) {
		fprintf( stderr, "ERROR: out of memory!\n");
		exit(EXIT_FAILURE);
	}
}




Titolo TitoloInit( void)
{
	Titolo titolo;
	titolo = (Titolo)malloc( sizeof(*titolo));
	memCheck( titolo);
	strcpy( titolo->code, "");
	titolo->quot_coll = BstInit();
	return titolo;
}

void TitoloFree( Titolo titolo)
{
	BstFree( titolo->quot_coll);
	free( titolo);
}

Titolo TitoloNull( void)
{
	return NULL;
}

int TitoloCheckNull( Titolo titolo)
{
	if( titolo == NULL)
		return 1;
	return 0;
}

quotazione_t TitoloSearchQuot( Titolo titolo, mytime_t date)
{
	return BstSearchByDate( titolo->quot_coll, date);
}




static link NewNode( Titolo titolo, link next)
{
	link new;
	new = (link)malloc(sizeof(*new));
	memCheck( new);
	new->next = next;
	new->titolo = titolo;
	return new;
}

TList ListInit( void)
{
	TList list;
	list = (TList)malloc( sizeof(*list));
	memCheck( list);
	list->head = NULL;
	list->n_titoli = 0;
	return list;
}


static void recursiveFree( link x)
{
	if( x == NULL)
		return;
	recursiveFree( x->next);
	TitoloFree( x->titolo);
	free( x);
}

void ListFree( TList list)
{
	recursiveFree( list->head);
	free( list);
}




void ListPrint( FILE *output, TList list)
{
	link x = list->head;
	for( int i = 0; i < list->n_titoli; i++) {
		fprintf( output, "Titolo: %s\n", x->titolo->code);
		fprintf( output, "Quotazioni Giornaliere:\n");
		BstPrint( output, x->titolo->quot_coll, IN_ORDER);
		fprintf( output, "\n");
		x = x->next;
	}
}


static void TitoloUpdate( Titolo dest, Titolo src)
{
	quotazione_t temp;
	int n_quot;

	n_quot = BstGetNodes( src->quot_coll);
	for( int i = 0; i < n_quot; i++) {
		temp = BstSelect( src->quot_coll, i);
		BstInsert( dest->quot_coll, temp);
	}
}

int ListAddTitolo( TList list, Titolo titolo)
{
	/* Lista ordinata in base al codice.  */
	link x, p;
	if( list == NULL)
		return 0;
	
	for( x = list->head, p = NULL;
		 x != NULL && strcmp( titolo->code, x->titolo->code) > 0;
		 p = x, x = x->next);

	// Viene gestito il caso in cui venga inserito un titolo gia'
	// presente nella lista
	if( x != NULL  &&  strcmp( titolo->code, x->titolo->code) == 0) {
		TitoloUpdate( x->titolo, titolo);
		return 0;
	}

	if( p == NULL) {
		list->head = NewNode( titolo, x);
		return 1;
	}
	p->next = NewNode( titolo, x);
	list->n_titoli++;
	return 1;
}


void ListLoad( const char *file_name, TList list)
{
	FILE *input;
	int n_titoli;
	int n_trans;
	char code_buff[MAX_CODE];
	transazione_t *trans;
	char data_buff[DATA_STR + CLOCK_STR];
	char clock_buff[CLOCK_STR];
	Titolo titolo_buff;

	input = fopen( file_name, "r");
	if( input == NULL) {
		fprintf( stderr, "File: \"%s\" not found!\n", file_name);
		exit(EXIT_FAILURE);
	}

	fscanf( input, "%d", &n_titoli);

	for( int i = 0; i < n_titoli; i++) {
		fscanf( input, "%s%d", code_buff, &n_trans);
		titolo_buff = TitoloInit();
		strcpy( titolo_buff->code, code_buff);

		trans = (transazione_t *)calloc( n_trans, sizeof(*trans));
		// Leggo tutte le transazione del titolo
		for( int j = 0; j < n_trans; j++) {
			fscanf( input, "%s%s%d%d", data_buff, clock_buff, &trans[j].value, &trans[j].number);

			// Preparo la stringa per essere letta da TIMEread()
			data_buff[strlen(data_buff)+1] = '\0';
			data_buff[strlen(data_buff)] = ' ';
			strcat( data_buff, clock_buff);

			TIMEreadFromString( data_buff, &trans[j].time);
		}
		// Creo le quotazioni e le inserisco nell'albero
		for( int j = 0; j < n_trans; j++) {
			quotazione_t quot;
			mytime_t time;
			int numeratore = 0;
			int denominatore = 0;

			if( trans[j].is_read == 0) {
				time = trans[j].time;
				for( int k = j; k < n_trans; k++) {
					if( TIMEdatecmp( time, trans[k].time) == 0) {
						numeratore += trans[k].value * trans[k].number;
						denominatore += trans[k].number;
						trans[k].is_read = 1;
					}
				}
				quot.value  = (float)numeratore/(float)denominatore;
				quot.scambi = denominatore;
				quot.time   = time;
				BstInsert( titolo_buff->quot_coll, quot);
			}
		}
		ListAddTitolo( list, titolo_buff);
		free( trans);
	}
}


Titolo ListSearchByCode( TList list, char *code)
{
	link x;

	for( x = list->head; x != NULL  &&  strcmp( code, x->titolo->code) >= 0;
		 x = x->next) {
		if( strcmp( code, x->titolo->code) == 0)
			return x->titolo;
	}
	return TitoloNull();
}


quotazione_t TitoloSearchMinBeetwenDate( Titolo titolo, mytime_t data1, mytime_t data2)
{
	return BstSearchMinBetweenDate( titolo->quot_coll, data1, data2);
}

quotazione_t TitoloSearchMaxBeetwenDate( Titolo titolo, mytime_t data1, mytime_t data2)
{
	return BstSearchMaxBetweenDate( titolo->quot_coll, data1, data2);
}



void TitoloBalceQuotTree( Titolo titolo, int soglia)
{
	BstBalance( titolo->quot_coll, soglia);
}
