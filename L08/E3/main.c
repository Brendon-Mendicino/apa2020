/* Brendon Mendicino, 271784 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEN_STR 52
#define MAX_LEN_LINE 300
#define MAX_FILE_NAME 200
#define MAX_ITEMS_INS_INV 8

/*
 * I codici sono univoci, dunque
 * ne scelgo uno che sia il NULL
 */
#define CODE_NULL "PG0000"


typedef enum {false, true} bool;

typedef enum {
	r_new,
	r_rm,
	r_menu,
	r_stat,
	r_quit
} comando_e;


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
	item_t *item[MAX_ITEMS_INS_INV];
} equip_t;

typedef struct {
	int n_items;
	item_t *item;
} listItems_t;

typedef struct {
	char code[MAX_LEN_STR];
	char name[MAX_LEN_STR];
	char class[MAX_LEN_STR];
	equip_t equip;
	stat_t stat;
} charater_t;

typedef struct nodeChar_t nodeChar_t, *linkChar;

struct nodeChar_t {
	charater_t character;
	linkChar next;
};

typedef struct {
	linkChar head;
	linkChar tail;
	int n_characters;
} listCharacter_t; //Avrei dovo chiamarla listCharacters_t



/* Errors */
void ERR_out_of_mem( void);

/* Main Function */
FILE *openWritingFile( char *file_name);
int readCommand( comando_e *cmd);
void printPrompt( void);
void flushStdin( void);
int readStringFromStdin( char *str);


/* Commands Functions */
void characterEquipmentMenu( listItems_t *listItems, listCharacter_t *listCh);

/* Stat */
void printStat( FILE *output, stat_t stat);
stat_t StatNull( void);


/* Characters */
listCharacter_t *mallocMenuChararacters( void);
charater_t *mallocCharacter( void);
void freeListCharacter( linkChar x);
void readCharFromFileAndInsert( char *file_name, listCharacter_t *listCh);
charater_t readCharacterFromFile( FILE *input);
void initMenuCharacters( listCharacter_t *listCh);
linkChar charListSortInsertCode( charater_t character, linkChar head, linkChar *tail_p);
linkChar newNode( charater_t character, linkChar node_p);
void printCharacter( FILE *output, charater_t ch);
void printListCharacters( FILE *output, linkChar head);
charater_t CharacterNull( void);
charater_t listSearchCode( linkChar head, char *key);
charater_t listExtractCode( linkChar *head_p, linkChar *tail_p, char *key);
charater_t readCharacterFromFile_NoCode( FILE *input);
linkChar listInsertTail( charater_t ch, linkChar tail);
int characterCompare( charater_t a, charater_t b);
void printCharacterEquip( FILE *outut, charater_t *ch);
void printCharacterWithStat( FILE *output, charater_t ch);


/* Items */
listItems_t *mallocMenuItems( void);
void freeMenuItems( listItems_t *listItems);
void loadNewItemsFromFile( char *file_name, listItems_t *listItems);
int readItemFromString( char *str, item_t *item);
void printItem( FILE *output, item_t item);
void printListItems( FILE *output, listItems_t *list);


/* Code */
char *codeGetFromChar( charater_t *character);
int codeCompare( char *a, char *b);
void codeNextFromTail( linkChar tail, char *next_code);
void addOneToCode( int pos, char *next_code);






int main(void)
{
	char items_file_name[MAX_FILE_NAME] = "inventario.txt";
	char char_file_name[MAX_FILE_NAME] = "pg.txt";
	bool loop = true;
	comando_e cmd;
	listItems_t *listItems;
	listCharacter_t *listCh;


	listCh = mallocMenuChararacters();
	initMenuCharacters( listCh);
	readCharFromFileAndInsert( char_file_name, listCh);

	listItems = mallocMenuItems();
	loadNewItemsFromFile( items_file_name, listItems);

	//TEST----------------------
	/*
	printListItems( stdout, listItems);
	putchar( '\n');
	printListCharacters( stdout, menuCh->head);
	printCharacter( stdout, menuCh->tail->character);
	*/


	while( loop) {
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
			charater_t *ch_buffer;
			ch_buffer = mallocCharacter();

			printf( "Format: [Name] [Class] [Hp][Mp][Atk][Def][Mag][Spr]\n");
			// Il codice sara' il successivo del character in coda
			*ch_buffer = readCharacterFromFile_NoCode( stdin);

			// Gestisco il caso in cui la lista sia vuota
			if( listCh->head == NULL) {
				strcpy( ch_buffer->code, "PG0001");
				listCh->head = newNode( *ch_buffer, NULL);
				listCh->tail = listCh->head;
				listCh->n_characters++;
			}
			// Caso normale
			else if( characterCompare( *ch_buffer, CharacterNull()) != 0) {
				// Il codice sara' il successivo dell'elemento in coda
				codeNextFromTail( listCh->tail, ch_buffer->code);
				listCh->tail = listInsertTail( *ch_buffer, listCh->tail);
			}
			else
				fprintf( stderr, "Failed to insert new character!\n");

			free(ch_buffer);
		}
		else if( cmd == r_rm)
		{
			charater_t *ch;
			char code_buffer[MAX_LEN_STR];

			printf( "Choose character you want to delete.\n");
			printf( "\"Number of characters: %d\"\n", listCh->n_characters);
			printListCharacters( stdout, listCh->head);
			printf( "Insert code: ");
			fflush( stdout);

			ch = mallocCharacter();
			readStringFromStdin( code_buffer);

			*ch = listExtractCode( &listCh->head, &listCh->tail, code_buffer);
			if( characterCompare( *ch, CharacterNull()) == 0)
				fprintf( stderr, "No match found!\n");
			else {
				printf( "Character extracted from list:\n");
				printCharacter( stdout, *ch);
				listCh->n_characters--;
			}
			free( ch);
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
				printListCharacters( stdout, listCh->head);
				printf( "(Type [back] to exit)\n");
				printf( "Choose character, insert code: ");

				readStringFromStdin( code_buffer);

				if( strcmp( code_buffer, "back") == 0)
					break;

				//Scorro la lista finche' non trovo il codice corrispondente
				for( x = listCh->head;
					 x != NULL && codeCompare( x->character.code, code_buffer) != 0;
					 x = x->next);

				if( x == NULL)
					fprintf( stderr, "No match found!\n");
				else {
					printCharacterWithStat( stdout, x->character);
				}
			}
		}

		// TEST-------------------
		/*
		printf( "------\n");
		printListCharacters( stdout, menuCh->head);
		if( menuCh->tail != NULL)
			printCharacter( stdout, menuCh->tail->character);
		printf( "------\n");
		*/
	}
	

	freeMenuItems( listItems);
	freeListCharacter( listCh->head);
	free( listCh);
	return 0;
}



void printPrompt( void)
{
	printf( "Choose option:\n");
	printf( "new_char  ~> Add new character\n");
	printf( "rm_char   ~> Remove character\n");
	printf( "menu      ~> Manage character equipment\n");
	printf( "stat      ~> Show character stat\n");
	printf( "quit\n");
	printf( "-> ");
	fflush(stdout);
}

void flushStdin( void)
{
	char a;
	while( (a = getchar()) != '\n' && a != EOF);
}

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

int readCommand( comando_e *cmd)
{
	char cmd_buffer[MAX_LEN_STR];
	char *cmd_commands[r_quit+1] = {
		"new_char",
		"rm_char",
		"menu",
		"stat",
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

void ERR_out_of_mem( void)
{
	fprintf( stderr, "ERROR: out of memory!\n");
	exit(EXIT_FAILURE);
}


FILE *openWritingFile( char *file_name)
{
	FILE *input;
	if( (input = fopen( file_name, "r")) == NULL) {
		fprintf( stderr, "File: \"%s\" not found!\n", file_name);
		exit(EXIT_FAILURE);
	}
	return input;
}


listItems_t *mallocMenuItems( void)
{
	listItems_t *listItems;

	listItems = (listItems_t *) malloc(sizeof( listItems_t));
	if( listItems == NULL)
		ERR_out_of_mem();

	return listItems;
}



void loadNewItemsFromFile( char *file_name, listItems_t *listItems)
{
	FILE *input;
	char line_buffer[MAX_LEN_LINE];
	
	input = openWritingFile( file_name);

	fgets( line_buffer, MAX_LEN_LINE, input);
	sscanf( line_buffer, "%d", &listItems->n_items);

	listItems->item = (item_t *) malloc(listItems->n_items * sizeof(item_t));
	if( listItems->item == NULL)
		ERR_out_of_mem();
	
	for( int i = 0; i < listItems->n_items; i++) {
		fgets( line_buffer, MAX_LEN_LINE, input);
		readItemFromString( line_buffer, &listItems->item[i]);
	}
	
	fclose(input);
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


void freeMenuItems( listItems_t *listItems)
{
	free( listItems->item);
	free( listItems);
}


void printItem( FILE *output, item_t item)
{
	fprintf( output, "%s %s ", item.name, item.type);
	printStat( output, item.stat);
}

void printListItems( FILE *output, listItems_t *list)
{
	for( int i = 0; i < list->n_items; i++) {
		fprintf( output, "%2d) ", i+1);
		printItem( output, list->item[i]);
	}
}

void printStat( FILE *output, stat_t stat)
{
	fprintf( output, "%d %d %d %d %d %d\n",
			stat.hp, stat.mp, stat.atk, stat.def, stat.mag, stat.spr);
}


listCharacter_t *mallocMenuChararacters( void)
{
	listCharacter_t *listCh;

	listCh = (listCharacter_t *) malloc(sizeof(listCharacter_t));
	if( listCh == NULL)
		ERR_out_of_mem();

	return listCh;
}


void initMenuCharacters( listCharacter_t *listCh)
{
	listCh->head = NULL;
	listCh->tail = NULL;
	listCh->n_characters = 0;
}


void readCharFromFileAndInsert( char *file_name, listCharacter_t *listCh)
{
	FILE *input;
	charater_t *temp;

	input = openWritingFile( file_name);
	
	temp = (charater_t *) malloc(sizeof(charater_t));
	do {
		*temp = readCharacterFromFile( input);
		// I codici sono univoci
		if( codeCompare( temp->code, CODE_NULL) != 0) {
			// Prima inserzione
			if( listCh->head == NULL) {
				listCh->head = newNode( *temp, NULL);
				listCh->tail = listCh->head;
				listCh->n_characters++;
			}
			else {
				listCh->head = charListSortInsertCode( *temp, listCh->head, &listCh->tail);
				listCh->n_characters++;
			}
		}
	} while( codeCompare( temp->code, CODE_NULL) != 0);
	
	free(temp);
	fclose( input);
}

linkChar newNode( charater_t character, linkChar node_p)
{
	nodeChar_t *new_node;
	new_node = (linkChar) malloc(sizeof(nodeChar_t));
	if( new_node == NULL)
		ERR_out_of_mem();

	new_node->character = character;
	new_node->next = node_p;

	return new_node;
}

linkChar charListSortInsertCode( charater_t character, linkChar head, linkChar *tail_p)
{
	// Caso con 'head==NULL' deve essere gia getsito
	linkChar x, prev;

	for( x = head->next, prev = head;
		 x != NULL && codeCompare( x->character.code, character.code) < 0;
		 prev = x, x = x->next);

	prev->next = newNode( character, x);
	if( x == NULL)
		*tail_p = prev->next;

	return head;
}

linkChar listInsertTail( charater_t ch, linkChar tail)
{
	linkChar x = newNode( ch, tail->next);
	tail->next = x;
	return x;
}



charater_t readCharacterFromFile( FILE *input)
{
	charater_t ch;
	char line_buffer[MAX_LEN_LINE];
	
	fgets( line_buffer, MAX_LEN_LINE, input);
	if( sscanf( line_buffer, "%s%s%s%d%d%d%d%d%d",
				ch.code, ch.name, ch.class,
				&ch.stat.hp, &ch.stat.mp, &ch.stat.atk,
				&ch.stat.def, &ch.stat.mag, &ch.stat.spr) != 9)
		return CharacterNull();

	return ch;
}

charater_t readCharacterFromFile_NoCode( FILE *input)
{
	charater_t ch;
	char line_buffer[MAX_LEN_LINE];
	
	fgets( line_buffer, MAX_LEN_LINE, input);
	if( sscanf( line_buffer, "%s%s%d%d%d%d%d%d",
				ch.name, ch.class,
				&ch.stat.hp, &ch.stat.mp, &ch.stat.atk,
				&ch.stat.def, &ch.stat.mag, &ch.stat.spr) != 8)
		return CharacterNull();

	return ch;
}

charater_t CharacterNull( void)
{
	charater_t ch;
	strcpy( ch.code, CODE_NULL);
	strcpy( ch.name, "");
	strcpy( ch.class, "");
	ch.stat = StatNull();
	ch.equip.n_items = 0;

	return ch;
}

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

char *codeGetFromChar( charater_t *character)
{
	return character->code;
}

int codeCompare( char *a, char *b)
{
	return (int)strcmp( a, b);
}

int characterCompare( charater_t a, charater_t b)
{
	if( strcmp( a.code, b.code) != 0)
		return 1;
	if( strcmp( a.name, b.name) != 0)
		return 1;
	if( strcmp( a.class, b.class) != 0)
		return 1;

	return 0;
}


void freeListCharacter( linkChar x)
{
	if( x == NULL)
		return;
	freeListCharacter( x->next);
	free(x);
}


void printListCharacters( FILE *output, linkChar head)
{
	linkChar x = head;
	while( x != NULL) {
		printCharacter( output, x->character);
		x = x->next;
	}
}

void printCharacter( FILE *output, charater_t ch)
{
	fprintf( output, "%s %s %s ", ch.code, ch.name, ch.class);
	printStat( output, ch.stat);
}


charater_t listSearchCode( linkChar head, char *key)
{
	linkChar x;
	if( head == NULL)
		return CharacterNull();

	for( x = head; x != NULL; x = x->next) {
		if( codeCompare( x->next->character.code, key) == 0) {
			return x->next->character;
		}
	}
	return CharacterNull();
}

void codeNextFromTail( linkChar tail, char *next_code)
{
	strcpy( next_code, tail->character.code);

	/*******************************
	 * code = [P| G| X| X| X| X];  *
	 *        [0| 1| 2| 3| 4| 5]   *
	 *******************************/

	addOneToCode( 5, next_code);
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

charater_t *mallocCharacter( void)
{
	charater_t *ch;
	
	ch = (charater_t *)malloc(sizeof(charater_t));
	if( ch == NULL)
		ERR_out_of_mem();
	
	return ch;
}


charater_t listExtractCode( linkChar *head_p, linkChar *tail_p, char *key)
{
	linkChar *xp, t, tail_flag;
	charater_t c;
	if( *head_p == NULL)
		return CharacterNull();
	
	for( xp = head_p, tail_flag = NULL; *xp != NULL; tail_flag = *xp, xp = &((*xp)->next)) {
		if( codeCompare( (*xp)->character.code, key) == 0) {
			// Il ciclo e' arrivato in coda, dunque va cambiata con la precedente
			if( *xp == *tail_p) {
				// 'tail_flag' punta al predecessore di '*xp'
				// Nel caso in cui il ciclo arriva ad estrarre l'elemento in coda
				// devo cambiare il volore di 'tail'  a quello precedente
				*tail_p = tail_flag;
			}

			t = *xp;
			*xp = (*xp)->next;
			c = t->character;
			free( t);
			return c;
		}
	}
	return CharacterNull();
}

void printCharacterEquip( FILE *output, charater_t *ch)
{
	fprintf( output, "Number of items: %d; max=8\n", ch->equip.n_items);
	fprintf( output, "Equipment--------------------\n");
	for( int i = 0; i < ch->equip.n_items; i++) {
		fprintf( output, "%2d) ", i+1);
		printItem( output, *ch->equip.item[i] );
	}
	fprintf( output, "-----------------------------\n");
}


void characterEquipmentMenu( listItems_t *listItems, listCharacter_t *listCh)
{
	linkChar x;
	char str_buffer[MAX_LEN_STR];
	int index_buffer = 0;
	bool char_loop = true, item_loop = true;

	while( char_loop) {
		printf( "\n==+ EQUIPMENT MENU +==\n");
		printListCharacters( stdout, listCh->head);
		printf( "(Type [back] to exit)\n");
		printf( "Choose character, insert code: ");
		fflush( stdout);

		readStringFromStdin( str_buffer);
		if( strcmp( str_buffer, "back") == 0)
			break;

		//Scorro la lista finche' non trovo il codice corrispondente
		for( x = listCh->head;
			 x != NULL && codeCompare( x->character.code, str_buffer) != 0;
			 x = x->next);

		if( x == NULL)
			fprintf( stderr, "No match found!\n");
		else {
			while( item_loop) {
				printf( "\n\n");
				printCharacter( stdout, x->character);
				printCharacterEquip( stdout, &x->character);
				printf( "\nChoose if you want to remove or add an item.\n");
				printf( "(Type [back] to exit)\n");
				printf( "[add] [remove]: ");

				readStringFromStdin( str_buffer);
				if( strcmp( str_buffer, "back") == 0)
					break;

				if( strcmp( str_buffer, "remove") == 0 && (x->character.equip.n_items > 0) ){
					printf( "Choose index: ");
					readStringFromStdin( str_buffer);

					if( sscanf( str_buffer, "%d", &index_buffer) != 1)
						break;
					if( !( index_buffer > 0 && index_buffer <= x->character.equip.n_items)) {
						fprintf( stderr, "No match found!\n");
						break;
					}

					index_buffer = index_buffer-1;
					x->character.equip.n_items--;
					x->character.equip.item[ index_buffer] = x->character.equip.item[ x->character.equip.n_items];
				}
				else if( strcmp( str_buffer, "add") == 0) {
					// Esco dal while se si e' raggiunto il massimo numero di oggetti
					if( x->character.equip.n_items == MAX_ITEMS_INS_INV) {
						printf( "Equipment is full!\n");
						break;
					}
					printf( "\nChoose item you want to equip:\n");
					printListItems( stdout, listItems);
					printf( "Insert index: ");

					readStringFromStdin( str_buffer);
					if( sscanf( str_buffer, "%d", &index_buffer) != 1)
							break;
					if( !( index_buffer > 0 && index_buffer <= listItems->n_items)) {
						fprintf( stderr, "No match found!\n");
						break;
					}
					
					index_buffer = index_buffer-1;
					x->character.equip.item[ x->character.equip.n_items] = &listItems->item[index_buffer];
					x->character.equip.n_items++;
				}
			}
		}
	}
}


void printCharacterWithStat( FILE *output, charater_t ch)
{
	fprintf( output, "%s %s %s ", ch.code, ch.name, ch.class);
	
	// Add equip stat to ch stat
	for( int i = 0; i < ch.equip.n_items; i++) {
		ch.stat.hp  += ch.equip.item[i]->stat.hp;
		ch.stat.mp  += ch.equip.item[i]->stat.mp;
		ch.stat.atk += ch.equip.item[i]->stat.atk;
		ch.stat.def += ch.equip.item[i]->stat.def;
		ch.stat.mag += ch.equip.item[i]->stat.mag;
		ch.stat.spr += ch.equip.item[i]->stat.spr;
	}
	
	// Se il valore e' minore di 0 viene impostato a 0
	if( ch.stat.hp  < 0) ch.stat.hp  = 0;
	if( ch.stat.mp  < 0) ch.stat.mp  = 0;
	if( ch.stat.atk < 0) ch.stat.atk = 0;
	if( ch.stat.def < 0) ch.stat.def = 0;
	if( ch.stat.mag < 0) ch.stat.mag = 0;
	if( ch.stat.spr < 0) ch.stat.spr = 0;

	printStat( stdout, ch.stat);
	printCharacterEquip( stdout, &ch);
}
