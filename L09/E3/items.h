/***  items.h  ***/

#ifndef __ITEMS_H
#define __ITEMS_H

#include <stdio.h>

#include "defines.h"

#define MAX_ITEMS_INS_INV 8

typedef struct {
	int hp;
	int mp;
	int atk;
	int def;
	int mag;
	int spr;
} stat_t;

typedef struct {
	char name[MAX_LEN_STR];
	char type[MAX_LEN_STR];
	stat_t stat;
} item_t;

typedef struct {
	int n_items;
	item_t *item;
} listItems_t;

typedef struct {
	int n_items;
	item_t *item[MAX_ITEMS_INS_INV];
} equip_t;




stat_t StatNull( void);

/* Stampa le Statisctiche.  */
void printStat( FILE *output, stat_t stat);

/* Copia Stat.  */
void statCopy( stat_t *dest, stat_t *src);

/* Costruttore Lista.  */
listItems_t *mallocListItems( void);

/* Distruttore Lista.  */
void freeListItems( listItems_t *listItems);

/* Stampa.  */
void printItem( FILE *output, item_t *item);
void printListItems( FILE *output, listItems_t *list);

/* Lettura da Stringa.  */
int readItemFromString( char *str, item_t *item);

/* Lettura da file.  */
void loadNewItemsFromFile( char *file_name, listItems_t *listItems);

/* Cerca 'item' per nome. La funzione deve ritornare NULL prima di procedere.  */
item_t *itemSearchByName( const char *name, listItems_t *list);

/* Rimuovi un 'item' dall''equip'.  */
int removeItemFromEquip( int index, equip_t *equip);

/* Aggiungi un 'item' nell''equip'.  */
int addItemToEquip( int index, listItems_t *list, equip_t *equip);

// TODO: inserire la ricerca di un oggetto per nome
//....

#endif  /* __ITEMS_H */
