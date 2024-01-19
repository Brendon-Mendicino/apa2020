
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "character.h"


character_t CharacterNull( character_t *old)
{
	character_t ch;
	strcpy( ch.code, CODE_NULL);
	strcpy( ch.name, "");
	strcpy( ch.class, "");
	ch.stat = StatNull();

	ch.equip = old->equip;
	ch.equip->item[0] = NULL;
	ch.equip->n_items = 0;

	return ch;
}

/* Costruttore Character.  */
character_t *mallocCharacter( void)
{
	character_t *ch;
	ch = (character_t *)malloc(sizeof(character_t));
	if( ch == NULL) {
		fprintf( stderr, "ERROR: out of memory!\n");
		exit(EXIT_FAILURE);
	}

	ch->equip = (equip_t *)malloc(sizeof(equip_t));
	if( ch->equip == NULL) {
		fprintf( stderr, "ERROR: out of memory!\n");
		exit(EXIT_FAILURE);
	}
	return ch;
}

/* Distruttore Character.  */
void freeCharacter( character_t *character)
{
	free( character->equip);
	free( character);
}

/* Copia Character.  */
void characterCopy( character_t *dest, character_t *src)
{
	strcpy( dest->code, src->code);
	strcpy( dest->name, src->name);
	strcpy( dest->class, src->class);
	statCopy( &dest->stat, &src->stat);
	for( int i = 0; i < MAX_ITEMS_INS_INV; i++)
		dest->equip->item[i] = src->equip->item[i];
	dest->equip->n_items = src->equip->n_items;
}

/* Costruisce ed inizializza la Lista.  */
listCharacter_t *initListChararacters( void)
{
	listCharacter_t *listCh;

	listCh = (listCharacter_t *) malloc(sizeof(listCharacter_t));
	if( listCh == NULL) {
		fprintf( stderr, "ERROR: out of memory!\n");
		exit(EXIT_FAILURE);
	}

	listCh->head = NULL;
	listCh->tail = NULL;
	listCh->n_characters = 0;
	return listCh;
}


void freeListCharacter_R( linkChar x)
{
	if( x == NULL)
		return;
	freeListCharacter_R( x->next);
	free(x->character.equip);
	free(x);
}



void addOneToCode( int pos, char *next_code)
{
	if( pos == 1) {
		return;
	}
	if( next_code[pos] == '9') {
		next_code[pos] = '0';
		if( next_code[pos-1] == '9')
			addOneToCode( pos-1, next_code);
		else
			next_code[pos-1]++;
	}
	else {
		next_code[pos]++;
	}
}

/* Aggiungi +1 all'ultimo 'code' della lista.  */
void codeNextFromTail( listCharacter_t *list, character_t *character)
{
	strcpy( character->code, list->tail->character.code);

	/*******************************
	 * code = [P| G| X| X| X| X];  *
	 *        [0| 1| 2| 3| 4| 5]   *
	 *******************************/

	addOneToCode( 5, character->code);
}





/* Distruttore Lista.  */
void freeListCharacter( listCharacter_t *list)
{
	freeListCharacter_R( list->head);
	free( list);
}


/* Create new node of the list.  */
linkChar newNode( character_t *character, linkChar node_p)
{
	nodeChar_t *new_node;
	new_node = (linkChar) malloc(sizeof(nodeChar_t));
	if( new_node == NULL) {
		fprintf( stderr, "ERROR: out of memory!\n");
		exit(EXIT_FAILURE);
	}
	/* La creazione di un nuovo nodo non modifica il campo 'equip'.  */
	new_node->character.equip = (equip_t *) malloc(sizeof(equip_t));
	if( new_node->character.equip == NULL) {
		fprintf( stderr, "ERROR: out of memory!\n");
		exit(EXIT_FAILURE);
	}

	characterCopy( &new_node->character, character);
	new_node->next = node_p;
	return new_node;
}

/* Distrugge un 'node' della Lista.  */
void freeNode( linkChar node_p)
{
	free( node_p->character.equip);
	free( node_p);
}

/* Inserisci un 'node' in testa alla Lista.  */
void charListInsertHead( character_t *character, listCharacter_t *list)
{
	/* Gestisce anche il caso di Lista vuota.  */
	if( list->head == NULL) {
		list->head = newNode( character, NULL);
		list->tail = list->head;
	}
	else {
		list->head = newNode( character, list->head->next);
	}
}

/* Inserisci un 'node' in coda della Lista.  */
void charListInsetTail( character_t *ch, listCharacter_t *list)
{
	linkChar x = newNode( ch, NULL);
	list->tail->next = x;
	list->tail = x;
}

/* Estrai un nodo dalla Lista, chiave = 'code'.  */
void charListExtractCode( char *key, listCharacter_t *list, character_t *ch)
{
	linkChar *xp, t, tail_flag;
	if( list->head == NULL) {
		*ch = CharacterNull( ch);
		return;
	}
	
	for( xp = &list->head, tail_flag = NULL; *xp != NULL; tail_flag = *xp, xp = &((*xp)->next)) {
		if( strcmp( (*xp)->character.code, key) == 0) {
			// Il ciclo e' arrivato in coda, dunque 'tail' va cambiata con il nodo precedente
			if( *xp == list->tail) {
				// 'tail_flag' punta al predecessore di '*xp'
				// Nel caso in cui il ciclo arriva ad estrarre l'elemento in coda
				// devo cambiare il volore di 'tail'  a quello precedente
				list->tail = tail_flag;
			}

			t = *xp;
			*xp = (*xp)->next;
			characterCopy( ch, &t->character);
			freeNode( t);
			return;
		}
	}
	*ch = CharacterNull( ch);
}

/* Inserzione ordinata in base al codice. Il caso head==NULL deve essere gia gestito.  */
void charListSortInsertByCode( character_t character, listCharacter_t *list)
{
	// Caso con 'head==NULL' deve essere gia getsito
	linkChar x, prev;

	for( x = list->head, prev = NULL;
		 x != NULL && strcmp( x->character.code, character.code) < 0;
		 prev = x, x = x->next);

	if( prev == NULL)
		list->head = newNode( &character, x);
	else
		prev->next = newNode( &character, x);

	if( x == NULL)
		list->tail = prev->next;
}


/* Lettura da di un 'character' da File, ritorna 0 se avviene un errore.  */
int readCharacterFromFile( FILE *input, character_t *ch)
{
	char line_buffer[MAX_LEN_LINE];
	
	fgets( line_buffer, MAX_LEN_LINE, input);
	if( sscanf( line_buffer, "%s%s%s%d%d%d%d%d%d",
				ch->code, ch->name, ch->class,
				&ch->stat.hp, &ch->stat.mp, &ch->stat.atk,
				&ch->stat.def, &ch->stat.mag, &ch->stat.spr) != 9) {
		return 0;
	}
	return 9;
}

/* Letture di un 'character' da File, senza 'code', ritorna 0 se avviene un errore.  */
int readCharacterFromFile_NoCode( FILE *input, character_t *ch)
{
	char line_buffer[MAX_LEN_LINE];
	
	fgets( line_buffer, MAX_LEN_LINE, input);
	if( sscanf( line_buffer, "%s%s%d%d%d%d%d%d",
				ch->name, ch->class,
				&ch->stat.hp, &ch->stat.mp, &ch->stat.atk,
				&ch->stat.def, &ch->stat.mag, &ch->stat.spr) != 8) {
		return 0;
	}
	return 8;
}

/* Letture ed inserzione in Lista da un File.  */
void readCharFromFileAndInsert( char *file_name, listCharacter_t *list)
{
	FILE *input;
	character_t *temp;

	input = fopen( file_name, "r");
	if( input == NULL) {
		fprintf( stderr, "File: \"%s\" not found!\n", file_name);
		exit(EXIT_FAILURE);
	}
	
	temp = mallocCharacter();

	do {
		if( readCharacterFromFile( input, temp) == 0)
			strcpy( temp->code, CODE_NULL);
		// I codici sono univoci
		if( strcmp( temp->code, CODE_NULL) != 0) {
			// Prima inserzione
			if( list->head == NULL) {
				list->head = newNode( temp, NULL);
				list->tail = list->head;
				list->n_characters++;
			}
			else {
				charListSortInsertByCode( *temp, list);
				list->n_characters++;
			}
		}
	} while( strcmp( temp->code, CODE_NULL) != 0);
	
	freeCharacter(temp);
	fclose( input);
}


/* Stampa un Character.  */
void printCharacter( FILE *output, character_t *ch)
{
	fprintf( output, "%s %s %s ", ch->code, ch->name, ch->class);
	printStat( output, ch->stat);
	fprintf( output, "\n");
}

/* Stampa l''equip' di un character.  */
void printCharacterEquip( FILE *output, character_t *ch)
{
	fprintf( output, "Number of items: %d; max=8\n", ch->equip->n_items);
	fprintf( output, "Equipment--------------------\n");
	for( int i = 0; i < ch->equip->n_items; i++) {
		fprintf( output, "%2d) ", i+1);
		printItem( output, ch->equip->item[i] );
		fprintf( output, "\n");
	}
	fprintf( output, "-----------------------------\n");
}

/* Stampa 'character' con le stat modificate dagli 'item'.  */
void printCharacterWithStat( FILE *output, character_t ch)
{
	fprintf( output, "%s %s %s ", ch.code, ch.name, ch.class);
	
	// Add equip stat to ch stat
	for( int i = 0; i < ch.equip->n_items; i++) {
		ch.stat.hp  += ch.equip->item[i]->stat.hp;
		ch.stat.mp  += ch.equip->item[i]->stat.mp;
		ch.stat.atk += ch.equip->item[i]->stat.atk;
		ch.stat.def += ch.equip->item[i]->stat.def;
		ch.stat.mag += ch.equip->item[i]->stat.mag;
		ch.stat.spr += ch.equip->item[i]->stat.spr;
	}
	
	// Se il valore e' minore di 0 viene impostato a 0
	if( ch.stat.hp  < 0) ch.stat.hp  = 0;
	if( ch.stat.mp  < 0) ch.stat.mp  = 0;
	if( ch.stat.atk < 0) ch.stat.atk = 0;
	if( ch.stat.def < 0) ch.stat.def = 0;
	if( ch.stat.mag < 0) ch.stat.mag = 0;
	if( ch.stat.spr < 0) ch.stat.spr = 0;

	printStat( stdout, ch.stat);
	fprintf( output, "\n");
	printCharacterEquip( stdout, &ch);
}

/* Stampa la Lista di Character.  */
void printListCharacters( FILE *output, listCharacter_t *list)
{
	linkChar x = list->head;
	while( x != NULL) {
		printCharacter( output, &x->character);
		x = x->next;
	}
}
