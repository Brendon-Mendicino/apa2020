/* Brendon Mendicino, 271784 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR 50
#define MAX_PROMPT_LINE_CHAR 300
#define MAX_CHAR_FILE_NAME 300


/*
#ifndef CLEAR_SCREEN

# ifdef __linux__
#  define CLEAR_SCREEN "clear"
# elif __unix__
#  define CLEAR_SCREEN "clear"
# elif defined(_WIN32) || defined(_WIN64)
#  define CLEAR_SCREEN "cls"
# elif __APPLE__
#  define CLEAR_SCREEN "clear"
# else
#  error "System not recognised!"
# endif

#endif // CLEAR_SCREEN
*/



#ifdef MAX_CHAR
/* MAX_CHAR + '\n' & '\0' */
# define MAX_CHAR_STRING (MAX_CHAR+2)
#endif


typedef enum {false, true} bool;

typedef enum {
	r_read,
	r_code_search,
	r_delete,
	r_date_delete,
	r_file_print,
	r_fine
} comando_e;

typedef enum {
	video,
	file
} input_type;


typedef struct {
	int g, m, a;
} data_t;


typedef struct {
	char codice[MAX_CHAR_STRING];
	char nome[MAX_CHAR_STRING];
	char cognome[MAX_CHAR_STRING];
	char via[MAX_CHAR_STRING];
	char citta[MAX_CHAR_STRING];
	data_t data;
	int cap;
} Item;

typedef struct node_t *link, node_t;

struct node_t{
	Item anagrafica;
	link next;
};


void flushStdin( void);
int readStringFromStdin( char *s);


/* Errors */
void ERR_out_of_mem( void);


/* Stampa */
void printList( FILE *output, link head);
void printPrompt( void);
void printItem( FILE *output, Item anagrafica);


/* leggi*  */
int leggiComando( comando_e *cmd);
int leggiInputType( input_type *input);


/* File r/w  */
void openWritingFileAndListPrint( link head);
FILE *openNewReadingFile( char *src);
FILE *openNewWritingFile( char *src);


/* Reading and Inserting */
void video_ReadNewListMemberAndInsert( link *head_p);
void file_ReadNewListMemberAndInsert( link *head_p);


/* Constructing */
Item *mallocItem( void);
link newNode( Item val, link next);


/* List Operation */
link listInsertHead( Item val, link old_head);
void listInsertInPosition( Item val, link head);
Item listExtractNode( link *head_p, node_t *key);
Item listExtractCode( link *head_p, char *key);
Item listSearchCode( link head, char *key);
link listSearchPreviousLink_GreaterThanData( link head, data_t key);
void listFree( node_t *node);

link listSortInsertData( Item val, link head);

void findSearchedCodeInsideList( link head);


/* Item Operation */
Item getItemFromFile( FILE *input);
int ItemCompare( Item a, Item b);
Item ITEMsetvoid( void);


/* Data Operation */
void deleteNode( node_t *node);
int dataCompare( data_t a, data_t b);
data_t dataSetVoid( void);
data_t dataGetFromItem( Item val);


/* Code Operation */
int codeCompare( char *a, char *b);
char *codeGetFromItem( Item *a);
data_t dataRead( void);






int main( void)
{
	link       head = NULL;
	bool       program_loop = true;
	comando_e  cmd;
	input_type input = video;

	/*
	system( CLEAR_SCREEN);
	*/
	while( program_loop)
	{
		printPrompt();
		if( !leggiComando( &cmd)) {
			fprintf( stderr, "Option not valid!\n\n");
			continue;
		}

		if( cmd == r_fine)
		{
			printf( "\n\n~~> Adios!\n");
			program_loop = false;
		}
		else if( cmd == r_read)
		{
			printf( "Choose input [video/file]: ");
			if( !leggiInputType( &input)) {
				fprintf( stderr, "Option not valid!\n\n");
				continue;
			}
			if( input == video)
				video_ReadNewListMemberAndInsert( &head);
			if( input == file)
				file_ReadNewListMemberAndInsert( &head);
		}
		else if( cmd == r_code_search)
		{
			printf( "Search by code [AXXXX]: ");
			findSearchedCodeInsideList( head);
		}
		else if( cmd == r_delete)
		{
			char code_pattern[MAX_CHAR_STRING];
			Item *temp;

			printf( "Element will be deleted by code [AXXXX]: ");
			readStringFromStdin( code_pattern);

			temp = mallocItem();
			*temp = listExtractCode( &head, code_pattern);
			if( ItemCompare( *temp, ITEMsetvoid()) == 0)
				fprintf( stderr, "No match found!\n");
			else {
				printf( "Deleted element from list:\n");
				printItem( stdout, *temp);
				printf("\n");
			}
			free( temp);
		}
		else if( cmd == r_date_delete)
		{
			Item *temp;
			link x;
			data_t a, b;
			bool mathc_found = false;

			printf( "Element will be deleted by date.\n");
			printf( "Insert beginning of range [dd/mm/yyyy]: ");
			a = dataRead();
			if( dataCompare( a, dataSetVoid()) == 0) {
				fprintf( stderr, "Opiton not valid!\n\n");
				continue;
			}
			printf( "Insert ending of range [dd/mm/yyyy]: ");
			b = dataRead();
			if( dataCompare( b, dataSetVoid()) == 0) {
				fprintf( stderr, "Opiton not valid!\n\n");
				continue;
			}
			
			temp = mallocItem();
			// Intervallo = [a, b]
			for( x = head; x != NULL; x = x->next) {
				if( dataCompare( dataGetFromItem( x->anagrafica), a) >= 0) {
					// 'x' potrebbe essere l'inizio dell'intervallo
					break;
				}
			}
			while( x != NULL) {
				link y = x->next;
				if( dataCompare( dataGetFromItem( x->anagrafica), b) <= 0) {
					*temp = listExtractNode( &head, x);
					printItem( stdout, *temp);
					mathc_found = true;
				}
				else break;
				x = y;
			}
			if( !mathc_found)
				fprintf( stderr, "No match found!\n");
			free(temp);
		}
		else if( cmd == r_file_print)
		{
			printf( "Choose output file, insert name: ");
			openWritingFileAndListPrint( head);
		}

		// Per test
		printList( stdout, head);
		putchar('\n');
	}

	listFree( head);

	return 0;
}



void printPrompt( void)
{
	printf( "Inserire un comando:\n");
	printf( "read         ~>Legge anagrafica\n");
	printf( "code_search  ~>Ricerca per codice\n");
	printf( "delete       ~>Cancellazione di un elemento\n");
	printf( "date_delete  ~>Cancellazione tra due date\n");
	printf( "file_print   ~>Stampa lista su file\n");
	printf( "fine\n");
	printf( "-> ");
	/* Forzo la stampa dell'output */
	fflush( stdout);
}

void printList( FILE *output, link head)
{
	link x;
	printf( "\n");
	for( x = head; x != NULL; x = x->next)
		printItem( output, x->anagrafica);
}

void printItem( FILE *output, Item anagrafica)
{
	fprintf( output, "%s %s %s %d/%d/%d %s %s %d\n",
			anagrafica.codice, anagrafica.nome, anagrafica.cognome,
			anagrafica.data.g, anagrafica.data.m, anagrafica.data.a,
			anagrafica.via, anagrafica.citta, anagrafica.cap
		  );
}

int leggiComando( comando_e *cmd)
{
	char cmd_buffer[MAX_CHAR_STRING];

	readStringFromStdin( cmd_buffer);
	
	char *cmd_comandi[r_fine+1] = {
		"read",
		"code_search",
		"delete",
		"date_delete",
		"file_print",
		"fine"
	};

	for( int i = 0; i < r_fine+1; i++) {
		if( strcmp( cmd_comandi[i], cmd_buffer) == 0) {
			*cmd = i;
			return 1;
		}
	}
	return 0;
}

int leggiInputType( input_type *input)
{
	char input_buffer[MAX_CHAR_STRING];

	char *input_choise[2] = {
		"video",
		"file"
	};

	readStringFromStdin( input_buffer);

	for( int i = 0; i < 2; i++) {
		if( strcmp( input_choise[i], input_buffer) == 0) {
			*input = i;
			return 1;
		}
	}
	return 0;
}


Item *mallocItem( void)
{
	Item *val;
	val = (Item *)malloc(sizeof(Item));
	if( val == NULL)
		ERR_out_of_mem();

	return val;
}


Item getItemFromFile( FILE *input)
{
	Item val;
	char line_buffer[MAX_PROMPT_LINE_CHAR];
	char data_buffer[MAX_CHAR_STRING];

	fgets( line_buffer, MAX_PROMPT_LINE_CHAR, input);
	if( sscanf( line_buffer, "%s%s%s%s%s%s%d",
				val.codice, val.nome, val.cognome, data_buffer,
				val.via, val.citta, &val.cap) != 7)
		return ITEMsetvoid();


	if( sscanf( data_buffer, "%d/%d/%d",
				&val.data.g, &val.data.m, &val.data.a) != 3)
		return ITEMsetvoid();

	return val;
}

void ERR_out_of_mem( void)
{
	fprintf( stderr, "ERROR: out of memory!\n");
	exit(EXIT_FAILURE);
}


Item ITEMsetvoid( void)
{
	Item val;
	strcpy( val.codice, "");
	strcpy( val.nome, "");
	strcpy( val.cognome, "");
	strcpy( val.via, "");
	strcpy( val.citta, "");
	val.data = dataSetVoid();
	val.cap = 0;

	return val;
}

data_t dataSetVoid( void)
{
	data_t data;
	data.g = 0;
	data.m = 0;
	data.a = 0;

	return data;
}


link newNode( Item val, link next)
{
	link temp = (link)malloc(sizeof(node_t));
	if( temp == NULL)
		ERR_out_of_mem();
	
	temp->anagrafica = val;
	temp->next = next;
	
	return temp;
}


link listSearchPreviousLink_GreaterThanData( link head, data_t key)
{
	/* Cerca il 'node' precedente a quello con la data maggiore di 'key' */
	link x, prev;
	if( head == NULL)
		return NULL;

	for( x = head, prev = NULL; x != NULL; prev = x, x = x->next) {
		if( dataCompare( dataGetFromItem( x->anagrafica), key) >= 0)
			return prev;
	}
	return prev;
}

int dataCompare( data_t a, data_t b)
{
	/* data_* sara'  2020|01|30 (yyyy|mm|gg) molto piu' facile da comparare */
	int data_a = a.a *10000 + a.m *100 + a.g;
	int data_b = b.a *10000 + b.m *100 + b.g;

	return (data_a - data_b);
}

data_t dataGetFromItem( Item val)
{
	data_t data = val.data;
	return data;
}


link listInsertHead( Item val, link old_head)
{
	link new_head;
	new_head = newNode( val, old_head);
	return new_head;
}



void listInsertInPosition( Item val, link head)
{
	link searched_element, new_element;

	searched_element = listSearchPreviousLink_GreaterThanData( head, val.data);
	new_element = newNode( val, searched_element->next);
	searched_element->next = new_element;
}


void listFree( node_t *node)
{
	if( node == NULL)
		return;
	listFree( node->next);
	deleteNode( node);
}

void deleteNode( node_t *node)
{
	free(node);
}


int ItemCompare( Item a, Item b)
{
	if( strcmp( a.codice, b.codice) != 0)
		return 1;
	if( strcmp( a.nome, b.nome) != 0)
		return 1;
	if( strcmp( a.cognome, b.cognome) != 0)
		return 1;
	if( strcmp( a.via, b.via) != 0)
		return 1;
	if( strcmp( a.citta, b.citta) != 0)
		return 1;
	if( dataCompare( a.data, b.data) != 0)
		return 1;
	if( a.cap != b.cap)
		return 1;

	return 0;
}



void video_ReadNewListMemberAndInsert( link *head_p)
{
	// La lista e' ordinata in base alla data
	Item *temp;

	printf( "Formato: [AXXXX] [Nome] [Cognome] [Data] [Via] [Citta'] [Cap]\n");
	temp = mallocItem();
	*temp = getItemFromFile( stdin);
	if( ItemCompare( *temp, ITEMsetvoid()) != 0) {
		*head_p = listSortInsertData( *temp, *head_p);
	}
	else
		fprintf( stderr, "Failed to insert new element!\n\n");

	free(temp);
}

void file_ReadNewListMemberAndInsert( link *head_p)
{
	// La lista e' ordinata in base alla data
	FILE *input_file;
	char file_name[MAX_CHAR_FILE_NAME];
	Item *temp;

	printf( "Inserire il nome del file: ");
	readStringFromStdin( file_name);

	input_file = openNewReadingFile( file_name);

	temp = mallocItem();
	do {
		*temp = getItemFromFile( input_file);
		if( ItemCompare( *temp, ITEMsetvoid()) != 0) {
			*head_p = listSortInsertData( *temp, *head_p);
			// Serve un 'link' precedente al nodo cercato con chiave 'gt_data'
			/*
			if( listSearchPreviousLink_GreaterThanData( *head_p, temp->data) == NULL)
				*head_p = listInsertHead( *temp, *head_p);
			else
				listInsertInPosition( *temp, *head_p);
			*/
		}
	} while( ItemCompare( *temp, ITEMsetvoid()) != 0);
	free(temp);
	fclose( input_file);
}

FILE *openNewReadingFile( char *src)
{
	FILE *input_file;
	if( (input_file = fopen( src, "r")) == NULL) {
		fprintf( stderr, "File: \"%s\" not found!\n", src);
		exit(EXIT_FAILURE);
	}
	return input_file;
}

void findSearchedCodeInsideList( link head)
{
	Item *temp;
	char code_pattern[MAX_CHAR_STRING];

	readStringFromStdin( code_pattern);

	temp = mallocItem();
	*temp = listSearchCode( head, code_pattern);
	
	if( ItemCompare( *temp, ITEMsetvoid()) == 0)
		fprintf( stderr, "No match found!\n");
	else
		printItem( stdout, *temp);

	free(temp);
}

Item listSearchCode( link head, char *key)
{
	link x;
	if( head == NULL)
		return ITEMsetvoid();

	for( x = head; x != NULL; x = x->next) {
		if( codeCompare( codeGetFromItem( &x->anagrafica), key) == 0)
			return x->anagrafica;
	}
	return ITEMsetvoid();
}

int codeCompare( char *a, char *b)
{
	return strcmp(a, b);
}

char *codeGetFromItem( Item *a)
{
	char *code = a->codice;
	return code;
}



void openWritingFileAndListPrint( link head)
{
	FILE *output_file;
	char file_name[MAX_CHAR_STRING];

	readStringFromStdin( file_name);

	output_file = openNewWritingFile( file_name);

	printList( output_file, head);

	fclose( output_file);
}

FILE *openNewWritingFile( char *src)
{
	FILE *output;
	output = fopen( src, "a");
	return output;
}

int readStringFromStdin( char *s)
{
	char line_buffer[MAX_PROMPT_LINE_CHAR];
	int arg_read;

	fgets( line_buffer, MAX_PROMPT_LINE_CHAR, stdin);
	arg_read = sscanf( line_buffer, "%s", s);

	/* Se '\n' non e' presente in 'line_buffer'
	 * si e' scritto oltre il limite
	 */
	if( strstr( line_buffer, "\n") == NULL)
		flushStdin();

	return arg_read;
}

void flushStdin( void)
{
	char a;
	while( (a = getchar()) != '\n' && a!=EOF);
}


Item listExtractCode( link *head_p, char *key)
{
	link *x_p, t;
	Item i = ITEMsetvoid();
	if( *head_p == NULL)
		return i;

	for( x_p = head_p; *x_p != NULL; x_p = &((*x_p)->next) ) {
		if( codeCompare( codeGetFromItem( &(*x_p)->anagrafica), key) == 0) {
			t = *x_p;
			*x_p = (*x_p)->next;
			i = t->anagrafica;
			free(t);
			return i;
		}
	}
	return i;
}


data_t dataRead( void)
{
	data_t a;
	char data_string[MAX_CHAR_STRING];

	readStringFromStdin( data_string);

	if( sscanf( data_string, "%d/%d/%d", &a.g, &a.m, &a.a) != 3)
		return dataSetVoid();

	return a;
}


Item listExtractNode( link *head_p, node_t *key)
{
	link *x_p, t;
	Item i = ITEMsetvoid();
	if( *head_p == NULL)
		return i;

	for( x_p = head_p; *x_p != NULL; x_p = &((*x_p)->next)) {
		if( *x_p == key) {
			t = *x_p;
			*x_p = (*x_p)->next;
			i = t->anagrafica;
			free(t);
			return i;
		}
	}
	return i;
}



link listSortInsertData( Item val, link head)
{
	link x, prev;
	data_t key = dataGetFromItem( val);
	if( head == NULL || dataCompare( dataGetFromItem( head->anagrafica), key) >= 0)
		return newNode( val, head);
	
	// Scorro la lista finche' non trovo un elemento piu grande || uguale
	for(x = head->next, prev = x;
		x != NULL && (dataCompare( dataGetFromItem( x->anagrafica), key) <= 0);
		prev = x, x = x->next);

	prev->next = newNode( val, x);
	return head;
}
