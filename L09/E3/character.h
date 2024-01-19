/***  character.h  ***/

#ifndef __CHARACTER_H
#define __CHARACTER_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "items.h"

/*
 * I codici sono univoci, dunque
 * ne scelgo uno che sia il NULL
 */
#define CODE_NULL "PG0000"


typedef struct {
	char code[MAX_LEN_STR];
	char name[MAX_LEN_STR];
	char class[MAX_LEN_STR];
	equip_t *equip;
	stat_t stat;
} character_t;


typedef struct nodeChar_t nodeChar_t, *linkChar;

struct nodeChar_t {
	character_t character;
	linkChar next;
};

typedef struct {
	linkChar head;
	linkChar tail;
	int n_characters;
} listCharacter_t;




// TODO: modificare tutte le chiamate di equip_t


character_t CharacterNull( character_t *old);

/* Costruttore Character.  */
character_t *mallocCharacter( void);

/* Distruttore Character.  */
void freeCharacter( character_t *character);

/* Costruisce ed inizializza la Lista.  */
listCharacter_t *initListChararacters( void);

/* Copia Character.  */
void characterCopy( character_t *dest, character_t *src);

void freeListCharacter_R( linkChar x);
/* Distruttore Lista.  */
void freeListCharacter( listCharacter_t *list);



void addOneToCode( int pos, char *next_code);

/* Aggiungi +1 all'ultimo 'code' della lista.  */
void codeNextFromTail( listCharacter_t *list, character_t *character);



/*****************************/
/*  Operazioni sulla lista.  */
/*****************************/

/* Nuovo nodo.  */
linkChar newNode( character_t *character, linkChar node_p);

/* Distrugge un 'node' della Lista.  */
void freeNode( linkChar node_p);

/* Inserisci un node in testa alla Lista.  */
void charListInsertHead( character_t *character, listCharacter_t *list);
/* Inserisci un 'node' in coda della Lista.  */
void charListInsetTail( character_t *ch, listCharacter_t *list);
/* Estrai un nodo dalla Lista, chiave = 'code'.  */
void charListExtractCode( char *key, listCharacter_t *list, character_t *ch);

/* Inserzione ordinata in base al codice. Il caso head==NULL deve essere gia gestito.  */
void charListSortInsertByCode( character_t character, listCharacter_t *list);


/* Lettura da di un 'character' da File, ritorna 0 se avviene un errore.  */
int readCharacterFromFile( FILE *input, character_t *ch);
/* Letture di un 'character' da File, senza 'code', ritorna 0 se avviene un errore.  */
int readCharacterFromFile_NoCode( FILE *input, character_t *ch);
/* Letture ed inserzione in Lista da un File.  */
void readCharFromFileAndInsert( char *file_name, listCharacter_t *list);

/* Stampa un Character.  */
void printCharacter( FILE *output, character_t *ch);

/* Stampa l''equip' di un character.  */
void printCharacterEquip( FILE *output, character_t *ch);

/* Stampa la Lista di Character.  */
void printListCharacters( FILE *output, listCharacter_t *list);

/* Stampa 'character' con le stat modificate dagli 'item'.  */
void printCharacterWithStat( FILE *output, character_t ch);


#endif  /* __CHARACTER_H */

