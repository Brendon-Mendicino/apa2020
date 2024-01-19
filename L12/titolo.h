
#ifndef TITOLO_H
#define TITOLO_H



#include "quotazione.h"


#define MAX_CODE  21

typedef struct titolo_t *Titolo;

typedef struct tlist_t *TList;



Titolo TitoloInit( void);

void TitoloFree( Titolo titolo);

Titolo TitoloNull( void);

int TitoloCheckNull( Titolo titolo);

quotazione_t TitoloSearchQuot( Titolo titolo, mytime_t date);


TList ListInit( void);

void ListFree( TList list);

int ListAddTitolo( TList list, Titolo titolo);

void ListLoad( const char *file_name, TList list);

void ListPrint( FILE *output, TList list);

Titolo ListSearchByCode( TList list, char *code);

quotazione_t TitoloSearchMinBeetwenDate( Titolo titolo, mytime_t data1, mytime_t data2);

quotazione_t TitoloSearchMaxBeetwenDate( Titolo titolo, mytime_t data1, mytime_t data2);

void TitoloBalceQuotTree( Titolo titolo, int solgia);


#endif  // TITOLO_H
