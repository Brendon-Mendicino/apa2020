/* Brendon Mendicino, 271784 */

#include <stdio.h>
#include <stdlib.h>

#include "defines.h"
#include "items.h"
#include "character.h"

typedef enum {false, true} bool;

typedef enum {
	r_new,
	r_rm,
	r_menu,
	r_stat,
	r_src_item,
	r_quit
} comando_e;




void flushStdin( void)
{
	char a;
	while( (a = getchar()) != '\n' && a != EOF);
}

/* Leggi una stringa da stdin.  */
int readStringFromStdin( char *str)
{
	char line_buffer[MAX_LEN_LINE];
	int arg_read;

	fgets( line_buffer, MAX_LEN_LINE, stdin);
	arg_read = sscanf( line_buffer, "%s", str);

	/*
	 * Se '\n' non e' presente in 'line_buffer'
	 * si e' scritto oltre il limite
	 */
	if( strstr( line_buffer, "\n") == NULL)
		flushStdin();
	
	return arg_read;
}



/* Stampa Opzioni.  */
void printPrompt( void)
{
	printf( "Choose option:\n");
	printf( "new_char  ~> Add new character\n");
	printf( "rm_char   ~> Remove character\n");
	printf( "menu      ~> Manage character equipment\n");
	printf( "stat      ~> Show character stat\n");
	printf( "src_item  ~> Search item by name\n");
	printf( "quit\n");
	printf( "-> ");
	fflush(stdout);
}

/* Legge comando, ritorna 0 in presenza di errore.  */
int readCommand( comando_e *cmd)
{
	char cmd_buffer[MAX_LEN_STR];
	char *cmd_commands[] = {
		"new_char",
		"rm_char",
		"menu",
		"stat",
		"src_item",
		"quit"
	};

	readStringFromStdin( cmd_buffer);

	for( int i = 0; i < r_quit+1; i++) {
		if( strcmp( cmd_buffer, cmd_commands[i]) == 0) {
			*cmd = i;
			return 1;
		}
	}
	return 0;
}


void characterEquipmentMenu( listItems_t *listItems, listCharacter_t *listCh)
{
	linkChar x;
	char str_buffer[MAX_LEN_STR];
	int index_buffer = 0;
	bool char_loop = true, item_loop = true;

	while( char_loop) {
		printf( "\n==+ EQUIPMENT MENU +==\n");
		printListCharacters( stdout, listCh);
		printf( "(Type [back] to exit)\n");
		printf( "Choose character, insert code: ");
		fflush( stdout);

		readStringFromStdin( str_buffer);
		if( strcmp( str_buffer, "back") == 0)
			break;

		//Scorro la lista finche' non trovo il codice corrispondente
		for( x = listCh->head;
			 x != NULL && strcmp( x->character.code, str_buffer) != 0;
			 x = x->next);

		if( x == NULL)
			fprintf( stderr, "No match found!\n");
		else {
			while( item_loop) {
				printf( "\n\n");
				printCharacter( stdout, &x->character);
				printCharacterEquip( stdout, &x->character);
				printf( "\nChoose if you want to remove or add an item.\n");
				printf( "(Type [back] to exit)\n");
				printf( "[add] [remove]: ");

				readStringFromStdin( str_buffer);
				if( strcmp( str_buffer, "back") == 0)
					break;

				if( strcmp( str_buffer, "remove") == 0 && (x->character.equip->n_items > 0) ){
					printf( "Choose index: ");
					readStringFromStdin( str_buffer);

					if( sscanf( str_buffer, "%d", &index_buffer) != 1)
						break;

					index_buffer = index_buffer-1;
					if( removeItemFromEquip( index_buffer, x->character.equip) == 0)
						break;
				}
				else if( strcmp( str_buffer, "add") == 0) {
					// Esco dal while se si e' raggiunto il massimo numero di oggetti
					if( x->character.equip->n_items == MAX_ITEMS_INS_INV) {
						printf( "Equipment is full!\n");
						break;
					}
					printf( "\nChoose item you want to equip:\n");
					printListItems( stdout, listItems);
					printf( "Insert index: ");

					readStringFromStdin( str_buffer);
					if( sscanf( str_buffer, "%d", &index_buffer) != 1)
							break;
					
					index_buffer = index_buffer-1;
					if( addItemToEquip( index_buffer, listItems, x->character.equip) == 0)
						break;
				}
			}
		}
	}
}



int main( void)
{
	char items_file_name[MAX_FILE_NAME] = "inventario.txt";
	char char_file_name[MAX_FILE_NAME] = "pg.txt";
	bool loop = true;
	comando_e cmd;
	listItems_t *listItems;
	listCharacter_t *listCh;

	
	listCh = initListChararacters();
	readCharFromFileAndInsert( char_file_name, listCh);

	listItems = mallocListItems();
	loadNewItemsFromFile( items_file_name, listItems);

	printListItems( stdout, listItems);
	printListCharacters( stdout, listCh);

	while( loop) {
		printf( "\n");
		printPrompt();
		if( !readCommand( &cmd)) {
			fprintf( stderr, "Option not valid!\n");
			continue;
		}

		if( cmd == r_quit)
		{
			loop = false;
		}
		else if( cmd == r_new)
		{
			character_t *ch_buffer;
			ch_buffer = mallocCharacter();

			printf( "Format: [Name] [Class] [Hp][Mp][Atk][Def][Mag][Spr]\n");
			// Il codice sara' il successivo del character in coda
			if( readCharacterFromFile_NoCode( stdin, ch_buffer) == 0) {
				fprintf( stderr, "Failed to insert new character!\n");
			}
			else {
				// Gestisco il caso in cui la lista sia vuota
				if( listCh->head == NULL) {
					strcpy( ch_buffer->code, "PG0001");
					charListInsertHead( ch_buffer, listCh);
					listCh->n_characters++;
				}
				// Caso normale
				else {
					// Il codice sara' il successivo dell'elemento in coda
					codeNextFromTail( listCh, ch_buffer);
					charListInsetTail( ch_buffer, listCh);
					listCh->n_characters++;
				}
			}
			freeCharacter( ch_buffer);
		}
		else if( cmd == r_rm)
		{
			character_t *ch_buffer;
			char code_buffer[MAX_LEN_STR];

			printf( "Choose character you want to delete.\n");
			printf( "\"Number of characters: %d\"\n", listCh->n_characters);
			printListCharacters( stdout, listCh);
			printf( "Insert code: ");
			fflush( stdout);

			ch_buffer = mallocCharacter();
			readStringFromStdin( code_buffer);

			charListExtractCode( code_buffer, listCh, ch_buffer);

			if( strcmp( ch_buffer->code, CODE_NULL) == 0)
				fprintf( stderr, "No match found!\n");
			else {
				printf( "Character extracted from list:\n");
				printCharacter( stdout, ch_buffer);
				putchar('\n');
				listCh->n_characters--;
			}
			freeCharacter( ch_buffer);
		}
		else if( cmd == r_menu)
		{
			characterEquipmentMenu( listItems, listCh);
		}
		else if( cmd == r_stat)
		{
			char code_buffer[MAX_LEN_STR];
			linkChar x;

			while(1) {
				printf( "\n==+ STAT MENU +==\n");
				printListCharacters( stdout, listCh);
				printf( "(Type [back] to exit)\n");
				printf( "Choose character, insert code: ");

				readStringFromStdin( code_buffer);

				if( strcmp( code_buffer, "back") == 0)
					break;

				//Scorro la lista finche' non trovo il codice corrispondente
				for( x = listCh->head;
					 x != NULL && strcmp( x->character.code, code_buffer) != 0;
					 x = x->next);

				if( x == NULL)
					fprintf( stderr, "No match found!\n");
				else {
					printCharacterWithStat( stdout, x->character);
				}
			}
		}
		else if( cmd == r_src_item)
		{
			char name_buffer[MAX_LEN_LINE];
			item_t *item;

			printf( "\n==+ ITEM SEARCH +==\n");
			printf( "Insert item name: ");

			readStringFromStdin( name_buffer);

			for( int i = 0; i < listItems->n_items; i++) {
				item = itemSearchByName( name_buffer, listItems);
				if( item == NULL)
					break;
				else {
					printf( "%2d) ", i+1);
					printItem( stdout, item);
					putchar('\n');
				}
			}
			putchar('\n');
		}

		printListCharacters( stdout, listCh);
	}

	freeListItems( listItems);
	freeListCharacter( listCh);
	return 0;
}
