
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "items.h"



stat_t StatNull( void)
{
	stat_t stat;

	stat.hp  = 0;
	stat.mp  = 0;
	stat.atk = 0;
	stat.def = 0;
	stat.mag = 0;
	stat.spr = 0;

	return stat;
}

/* Stampa le Statisctiche.  */
void printStat( FILE *output, stat_t stat)
{
	fprintf( output, "%d %d %d %d %d %d",
			stat.hp, stat.mp, stat.atk, stat.def, stat.mag, stat.spr);
}

/* Copia Stat.  */
void statCopy( stat_t *dest, stat_t *src)
{
	dest->hp  = src->hp;
	dest->mp  = src->mp;
	dest->atk = src->atk;
	dest->def = src->def;
	dest->mag = src->mag;
	dest->spr = src->spr;
}


void printItem( FILE *output, item_t *item)
{
	fprintf( output, "%s %s", item->name, item->type);
	fprintf( output, " ");
	printStat( output, item->stat);
}

void printListItems( FILE *output, listItems_t *list)
{
	for( int i = 0; i < list->n_items; i++) {
		fprintf( output, "%2d) ", i+1);
		printItem( output, &list->item[i]);
		printf( "\n");
	}
}

/* Costruttore Lista.  */
listItems_t *mallocListItems( void)
{
	listItems_t *listItems;

	listItems = (listItems_t *) malloc(sizeof( listItems_t));
	if( listItems == NULL) {
		fprintf( stderr, "ERROR: out of memory!\n");
		exit(EXIT_FAILURE);
	}

	listItems->item = NULL;
	listItems->n_items = 0;
	return listItems;
}

/* Distruttore Lista.  */
void freeListItems( listItems_t *listItems)
{
	free( listItems->item);
	free( listItems);
}


int readItemFromString( char *str, item_t *item)
{
	if( sscanf( str, "%s%s%d%d%d%d%d%d",
			item->name, item->type,
			&item->stat.hp, &item->stat.mp, &item->stat.atk,
			&item->stat.def, &item->stat.mag, &item->stat.spr) != 8)
		return 0;
	return 1;
}


/* Lettura da file.  */
void loadNewItemsFromFile( char *file_name, listItems_t *listItems)
{
	FILE *input;
	char line_buffer[MAX_LEN_LINE];

	input = fopen( file_name, "r");
	if( input == NULL) {
		fprintf( stderr, "File: \"%s\" not found!\n", file_name);
		exit(EXIT_FAILURE);
	}

	fgets( line_buffer, MAX_LEN_LINE, input);
	sscanf( line_buffer, "%d", &listItems->n_items);

	listItems->item = (item_t *) malloc(listItems->n_items * sizeof(item_t));
	if( listItems->item == NULL) {
		fprintf( stderr, "ERROR: out of memory!\n");
		exit(EXIT_FAILURE);
	}

	if( listItems->item == NULL) {
		fprintf( stderr, "ERROR: out of memory!\n");
		exit( EXIT_FAILURE);
	}

	for( int i = 0; i < listItems->n_items; i++) {
		fgets( line_buffer, MAX_LEN_LINE, input);
		readItemFromString( line_buffer, &listItems->item[i]);
	}
	
	fclose(input);
}

/* Cerca 'item' per nome. La funzione deve ritornare NULL prima di procedere.  */
item_t *itemSearchByName( const char *name, listItems_t *list)
{
	static int item_count = 0;
	int i, j;
	char name_buffer[MAX_LEN_STR];
	char list_name_buffer[MAX_LEN_STR];

	for( i = 0; i < (int)strlen(name); i++)
		name_buffer[i] = tolower( name[i]);
	name_buffer[i] = '\0';

	for( ; item_count < list->n_items; item_count++) {
		for( j = 0; j < (int)strlen( list->item[item_count].name); j++)
			list_name_buffer[j] = tolower( list->item[item_count].name[j]);
		list_name_buffer[j] = '\0';

		if( strstr( list_name_buffer, name_buffer) != NULL) {
			item_count++;
			return &(list->item[item_count-1]);
		}
	}
	item_count = 0;
	return NULL;
}


/* Rimuovi un 'item' dall''equip'.  */
int removeItemFromEquip( int index, equip_t *equip)
{
	if( !( index >= 0 && index < equip->n_items)) {
		fprintf( stderr, "No match found!\n");
		return 0;
	}
	equip->n_items--;
	equip->item[ index] = equip->item[ equip->n_items];
	return 1;
}

/* Aggiungi un 'item' nell''equip'.  */
int addItemToEquip( int index, listItems_t *list, equip_t *equip)
{
	if( !( index >= 0 && index < list->n_items)) {
		fprintf( stderr, "No match found!\n");
		return 0;
	}
	equip->item[equip->n_items] = &list->item[index];
	equip->n_items++;
	return 1;
}
