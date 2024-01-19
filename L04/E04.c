/* Brendon Mendicino, 271784 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STR 30
#define MAX_CORSE 1000

typedef enum {false, true} bool;

typedef enum {vid, file} tipo_output;

typedef enum {
	lineare,
	dicotomica
} ricerca;

typedef enum {
	r_ord_data,
	r_ord_codice,
	r_ord_stazione_part,
	r_ord_stazione_arr,
	r_ricerca_stazione_part,
	r_fine
} comando_e;


typedef struct {
	int g;
	int m;
	int a;
} Data;

typedef struct {
	char codice[MAX_STR];
	char partenza[MAX_STR];
	char dest[MAX_STR];
	Data data;
	char ora_part[MAX_STR];
	char ora_arr[MAX_STR];
	int ritardo;
	int is_search;
} Tratta;



void apri_file( FILE **input, char *str);
void salva_log( FILE *input, Tratta *tratta, int n_tratte);
void stampa_prompt( void);
bool leggiComando( comando_e *cmd, tipo_output *output_t);
void menu_ordinaTratte( Tratta *tratta, int n_tratte, comando_e cmd, ricerca *stato_ricerca);
void stampaTratta( Tratta *tratta, int n_tratte, tipo_output output_t);

/* algoritmi di sorting */
void bubbleSort_dataTratta( Tratta *tratta, int n_tratte);
void bubbleSort_codiceTratta( Tratta *t, int n);
void bubbleSort_stazionePartenza( Tratta *t, int n);
void bubbleSort_stazioneArrivo( Tratta *t, int n);

/* Manipolazione Tratta */
int TrattaData_gt( Tratta a, Tratta b);
int TrattaCodice_gt( Tratta a, Tratta b);
int TrattaStazionePartenza_gt( Tratta a, Tratta b);
int TrattaStazioneArrivo_gt( Tratta a, Tratta b);
void Tratta_cpy( Tratta *a, Tratta *b);

void RicercaDicotomicaTratta( Tratta *tratta, char *str, int n_tratte);
void RicercaLineareTratta( Tratta *tratta, char *str, int n_tratte);
int ricercaDicotomica( Tratta *tratta, char *str, int l, int r);

void stampaRicercaTratta( Tratta *t, int n, tipo_output output_t);


int main(void)
{
	FILE *input;
	Tratta tratta[MAX_CORSE] = {{{0}}};
	bool loop = true;
	int n_tratte = 0;
	comando_e cmd;
	ricerca stato_ricerca = lineare;
	tipo_output output_t;
	
	apri_file( &input, "corse.txt");
	fscanf( input, "%d", &n_tratte);
	salva_log( input, tratta, n_tratte);

	while(loop) {
		stampa_prompt();
		if( leggiComando( &cmd, &output_t) == false) {
			printf( "Opzione non valida!\n\n");
			continue;
		}
		if( cmd == r_fine) loop = false;

		menu_ordinaTratte( tratta, n_tratte, cmd, &stato_ricerca);

		if( cmd != r_ricerca_stazione_part && loop)
			stampaTratta( tratta, n_tratte, output_t);
		else
			stampaRicercaTratta( tratta, n_tratte, output_t);
	}


	fclose(input);
	return 0;
}


void apri_file( FILE **input, char *str)
{
	if( (*input = fopen( str, "r")) == NULL) {
		fprintf( stderr, "File: \"%s\" non trovato!\n", str);
		exit(1);
	}
}

void stampa_prompt( void)
{
	printf( "\n");
	printf( "Inserire un comando:\n");
	printf( "data [vid/file]\n");
	printf( "codice [vid/file]\n");
	printf( "stazione_partenza [vid/file]\n");
	printf( "stazione_arrivo [vid/file]\n");
	printf( "ricerca_stazione_partenza [vid/file]\n");
	printf( "fine\n");
	printf( "-> ");
}




void salva_log( FILE *input, Tratta *tratta, int n_tratte)
{
	int i = 0;
	char data_buff[MAX_STR] = {0};

	for( i = 0; i < n_tratte; i++) {
		fscanf( input, "%s%s%s%s%s%s%d",
				tratta[i].codice, tratta[i].partenza, tratta[i].dest,
				data_buff, tratta[i].ora_part, tratta[i].ora_arr,
				&tratta[i].ritardo);

		sscanf( data_buff, "%d/%d/%d",
				&tratta[i].data.g, &tratta[i].data.m, &tratta[i].data.a);
	}
}


bool leggiComando( comando_e *cmd, tipo_output *output_t)
{
	char vid_file_buff[MAX_STR] = {0};
	char word_cmd_buff[MAX_STR] = {0};

	scanf( "%s", word_cmd_buff);
	if( strcmp( word_cmd_buff, "fine") == 0) {
		*cmd = r_fine;
		return true;
	}

	scanf( "%s", vid_file_buff);

	char *comandi_cmd[5] = {
		"data",
		"codice",
		"stazione_partenza",
		"stazione_arrivo",
		"ricerca_stazione_partenza",
	};


	if( strcmp( vid_file_buff, "vid") == 0)
		*output_t = vid;
	else if( strcmp( vid_file_buff, "file") == 0)
		*output_t = file;
	else
		return false;


	for( int i = 0; i < r_fine; i++) {
		if( strcmp( word_cmd_buff, comandi_cmd[i]) == 0) {
			*cmd = i;
			return true;
		}
	}
	return false;
}






void menu_ordinaTratte( Tratta *tratta, int n_tratte, comando_e cmd, ricerca *stato_ricerca)
{
	char word_buff[MAX_STR] = {0};

	switch (cmd) {
		case r_ord_data:
			bubbleSort_dataTratta( tratta, n_tratte);
			*stato_ricerca = lineare;
			break;
		case r_ord_codice:
			bubbleSort_codiceTratta( tratta, n_tratte);
			*stato_ricerca = lineare;
			break;
		case r_ord_stazione_part:
			bubbleSort_stazionePartenza( tratta, n_tratte);
			*stato_ricerca = dicotomica;
			break;
		case r_ord_stazione_arr:
			bubbleSort_stazioneArrivo( tratta, n_tratte);
			*stato_ricerca = lineare;
			break;
		case r_ricerca_stazione_part:
			/* I campi 'is_search', di 'tratta', verranno modificati in '1',
			   di modo che la funzione 'stampaRicercaTratte'
			   stampi solo le tratte selezionate */
			printf( "Inserire il campo di ricerca: ");
			scanf( "%s", word_buff);
			if( *stato_ricerca == dicotomica) {
				RicercaDicotomicaTratta( tratta, word_buff, n_tratte);
			}
			else {
				RicercaLineareTratta( tratta, word_buff, n_tratte);
			}
			break;
		case r_fine:
			printf("\n-->> Bye!\n");
			break;
	}
}





void bubbleSort_dataTratta( Tratta *tratta, int n_tratte)
{
	int i = 0, j = 0;
	Tratta temp;

	for( i = 0; i < n_tratte; i++) {
		for( j = 0; j < n_tratte-i-1; j++) {
			if( TrattaData_gt( tratta[j], tratta[j+1])) {
				Tratta_cpy( &temp, &tratta[j]);
				Tratta_cpy( &tratta[j], &tratta[j+1]);
				Tratta_cpy( &tratta[j+1], &temp);
			}
		}
	}
}


void bubbleSort_codiceTratta( Tratta *t, int n)
{
	int i = 0, j = 0;
	Tratta temp;

	for( i = 0; i < n; i++) {
		for( j = 0; j < n-i-1; j++) {
			if( TrattaCodice_gt( t[j], t[j+1])) {
				Tratta_cpy( &temp, &t[j]);
				Tratta_cpy( &t[j], &t[j+1]);
				Tratta_cpy( &t[j+1], &temp);
			}
		}
	}
}


void bubbleSort_stazionePartenza( Tratta *t, int n)
{
	int i = 0, j = 0;
	Tratta temp;

	for( i = 0; i < n; i++) {
		for( j = 0; j < n-i-1; j++) {
			if( TrattaStazionePartenza_gt( t[j], t[j+1])) {
				Tratta_cpy( &temp, &t[j]);
				Tratta_cpy( &t[j], &t[j+1]);
				Tratta_cpy( &t[j+1], &temp);
			}
		}
	}
}


void bubbleSort_stazioneArrivo( Tratta *t, int n)
{
	int i = 0, j = 0;
	Tratta temp;

	for( i = 0; i < n; i++) {
		for( j = 0; j < n-i-1; j++) {
			if( TrattaStazioneArrivo_gt( t[j], t[j+1])) {
				Tratta_cpy( &temp, &t[j]);
				Tratta_cpy( &t[j], &t[j+1]);
				Tratta_cpy( &t[j+1], &temp);
			}
		}
	}
}




/*********
  
  Manipolazione Tratta

*********/

int TrattaData_gt( Tratta a, Tratta b)
{
	/* int_* sara'  2020|01|30  molto piu' facile da comparare */
	int data_a = a.data.a *10000 + a.data.m *100 + a.data.g;
	int data_b = b.data.a *10000 + b.data.m *100 + b.data.g;

	if( data_a > data_b)
		return 1;
	else if( data_a == data_b) {
		if( strcmp( a.ora_part, b.ora_part) > 0)
			return 1;
	}
	return 0;
}

int TrattaCodice_gt( Tratta a, Tratta b)
{
	/* Si assume che il 'codice' sia solo 'digit' */
	int x = atoi( a.codice);
	int y = atoi( b.codice);
	if( x > y) return 1;
	return 0;
}

int TrattaStazionePartenza_gt( Tratta a, Tratta b)
{
	if( strcmp( a.partenza, b.partenza) > 0) return 1;
	return 0;
}

int TrattaStazioneArrivo_gt( Tratta a, Tratta b)
{
	if( strcmp( a.dest, b.dest) > 0) return 1;
	return 0;
}

void Tratta_cpy( Tratta *a, Tratta *b)
{
	Tratta temp;


	strcpy( temp.codice, a->codice);
	strcpy( temp.partenza, a->partenza);
	strcpy( temp.dest, a->dest);
	temp.data.a = a->data.a;
	temp.data.m = a->data.m;
	temp.data.g = a->data.g;
	strcpy( temp.ora_part, a->ora_part);
	strcpy( temp.ora_arr, a->ora_arr);
	temp.ritardo = a->ritardo;


	strcpy( a->codice, b->codice);
	strcpy( a->partenza, b->partenza);
	strcpy( a->dest, b->dest);
	a->data.a = b->data.a;
	a->data.m = b->data.m;
	a->data.g = b->data.g;
	strcpy( a->ora_part, b->ora_part);
	strcpy( a->ora_arr, b->ora_arr);
	a->ritardo = b->ritardo;


	strcpy( b->codice, temp.codice);
	strcpy( b->partenza, temp.partenza);
	strcpy( b->dest, temp.dest);
	b->data.a = temp.data.a;
	b->data.m = temp.data.m;
	b->data.g = temp.data.g;
	strcpy( b->ora_part, temp.ora_part);
	strcpy( b->ora_arr, temp.ora_arr);
	b->ritardo = temp.ritardo;
}



/******

  Stampa

******/

void stampaTratta( Tratta *t, int n_tratte, tipo_output output_t)
{
	FILE *output;
	bool is_file_open = false;
	if(	output_t == vid)
		output = stdout;
	else {
		char buff[MAX_STR] = {0};
		is_file_open = true;
		printf( "Inserire il nome del file: ");
		scanf( "%s", buff);
		output = fopen( buff, "a");
	}

	for( int i = 0; i < n_tratte; i++) {
		fprintf( output, "%s %s %s %d/%d/%d %s %s %d\n",
				t[i].codice, t[i].partenza, t[i].dest, t[i].data.g, t[i].data.m, t[i].data.a,
				t[i].ora_part, t[i].ora_arr, t[i].ritardo);
	}

	if( is_file_open)
		fclose(output);
}

void stampaRicercaTratta( Tratta *t, int n, tipo_output output_t)
{
	FILE *output;
	bool is_file_open = false;
	if(	output_t == vid)
		output = stdout;
	else {
		char buff[MAX_STR] = {0};
		is_file_open = true;
		printf( "Inserire il nome del file: ");
		scanf( "%s", buff);
		output = fopen( buff, "a");
	}

	int i = 0;
	for( i = 0; i < n; i++) {
		if( t[i].is_search == 1) {
			fprintf( output, "%s %s %s %d/%d/%d %s %s %d\n",
					t[i].codice, t[i].partenza, t[i].dest, t[i].data.g, t[i].data.m, t[i].data.a,
					t[i].ora_part, t[i].ora_arr, t[i].ritardo);
			t[i].is_search = 0;
		}
	}

	if( is_file_open)
		fclose(output);
}






void RicercaDicotomicaTratta( Tratta *t, char *str, int n)
{
	int i = 0;
	int m = 0;
	m = ricercaDicotomica( t, str, 0, n);
	if( m == -1) return;
	i = m;

	/* porto i valori 'is_search' ad '1' di tutte le stringhe
	   che corrispondono al criterio di ricerca */
	while( i >= 0  &&  strncmp( str, t[i].partenza, strlen(str)) == 0) {
		t[i].is_search = 1;
		i--;
	}
	i = m+1;
	while( i < n  &&  strncmp( str, t[i].partenza, strlen(str)) == 0) {
		t[i].is_search = 1;
		i++;
	}
}

int ricercaDicotomica( Tratta *t, char *str, int l, int r)
{
	int m = (r+l)/2;
	if( l == r && strncmp( str, t[l].partenza, strlen(str)) == 0)
		return r;
	else if( l == r)
		return -1;
	
	if( strncmp( str, t[m].partenza, strlen(str)) == 0) return m;

	if( strncmp( str, t[m].partenza, strlen(str)) > 0) {
		return ricercaDicotomica( t, str, m+1, r);
	}
	return ricercaDicotomica( t, str, l, m);

	return -1;
}

void RicercaLineareTratta( Tratta *t, char *str, int n)
{
	int i = 0;
	for( i = 0; i < n; i++) {
		if( strncmp( str, t[i].partenza, strlen(str)) == 0)
			t[i].is_search = 1;
	}
}
