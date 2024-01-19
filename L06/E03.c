/* Brendon Mendicino, 271784 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STR 30
#define MAX_PROMPT_LINE_CHAR 200

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
	r_open_file,
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


// r_ord_stazione_arr+1 == numero di ordinamenti della tratte
const int n_ordinamenti = r_ord_stazione_arr+1;



/* Main Functions */
void salva_log( FILE *input, Tratta *tratta, int n_tratte);
Tratta *mallocTratta( FILE *input, int *n_tratte);
Tratta **mallocOrdinamentiTratta( int n_tratte);
FILE *openNewFile( char *input_file_name);
void copiaTrattaInOrdinamentiTratta( Tratta **ord_t, Tratta *t, int n);
bool leggiComando( comando_e *cmd, tipo_output *output_t);
void freeTratta( Tratta *t, Tratta **ord_t[n_ordinamenti]);
FILE *writingFileName( void);

/* Menu */
void menu_ordinaTratte(
		Tratta **ord_t[n_ordinamenti],
		int n_tratte,
		comando_e cmd,
		ricerca *stato_ricerca,
		char *input_file_name
		);

/* Algoritmi di Sorting */
void bubbleSort_dataTratta( Tratta **t, int n);
void bubbleSort_codiceTratta( Tratta **t, int n);
void bubbleSort_stazionePartenza( Tratta **t, int n);
void bubbleSort_stazioneArrivo( Tratta **t, int n);

/* Manipolazione Tratta */
int isDataTratta_gt( Tratta a, Tratta b);
int isCodiceTratta_gt( Tratta a, Tratta b);
int isStazionePartenzaTratta_gt( Tratta a, Tratta b);
int isStazioneArrivoTratta_gt( Tratta a, Tratta b);

/* Algoritmi di Ricerca */
void RicercaDicotomicaTratta( Tratta **tratta, char *str, int n_tratte);
void RicercaLineareTratta( Tratta **tratta, char *str, int n_tratte);
int  ricercaDicotomica( Tratta **tratta, char *str, int l, int r);

/* Stampa */
void stampa_nome_file( char *file_name);
void stampa_prompt( void);
void stampaTrattaOrdinata( Tratta **t[n_ordinamenti], int n_tratte, comando_e cmd, tipo_output output_t);
void stampaDescrizioneOrdinamento( FILE **output, comando_e cmd);
void stampaRicercaTratta( Tratta *t, int n, tipo_output output_t);
void ERR_out_of_mem(void);



int main(void)
{
	FILE *input;
	Tratta *tratta;
	Tratta **ordinamenti_tratta[n_ordinamenti];

	char         input_file_name[MAX_STR] = "corse.txt";
	bool         loop = true;
	int	         n_tratte = 0;
	ricerca      stato_ricerca = lineare;
	comando_e    cmd = r_ord_data;
	tipo_output  output_t;
	

	input = openNewFile( input_file_name);
	tratta = mallocTratta( input, &n_tratte);
	salva_log( input, tratta, n_tratte);
	fclose(input);
	for( int i = 0; i < n_ordinamenti; i++) {
		ordinamenti_tratta[i] = mallocOrdinamentiTratta( n_tratte);
		copiaTrattaInOrdinamentiTratta( ordinamenti_tratta[i], tratta, n_tratte);
	}

	while(loop) {
		stampa_nome_file( input_file_name);
		stampa_prompt();
		if( leggiComando( &cmd, &output_t) == false) {
			fprintf( stderr, "Opzione non valida!\n\n");
			continue;
		}

		menu_ordinaTratte( ordinamenti_tratta, n_tratte, cmd, &stato_ricerca, input_file_name);

		if( cmd == r_open_file) {
			freeTratta( tratta, ordinamenti_tratta);

			input = openNewFile( input_file_name);
			tratta = mallocTratta( input, &n_tratte);
			salva_log( input, tratta, n_tratte);
			fclose(input);
			for( int i = 0; i < n_ordinamenti; i++) {
				ordinamenti_tratta[i] = mallocOrdinamentiTratta( n_tratte);
				copiaTrattaInOrdinamentiTratta( ordinamenti_tratta[i], tratta, n_tratte);
			}
			continue;
		}
		if( cmd == r_fine) {
			freeTratta( tratta, ordinamenti_tratta);
			loop = false;
		}

		/*Uso 'tratta', dato che i puntatori a 'tratta' avranno modificato
		 *il campo 'is_search' in '1' dei vettori corrsipondeti al criterio di
		 *ricerca, verranno riportati a '0' all'interno della funzione
		 */
		if( cmd != r_ricerca_stazione_part && loop)
			stampaTrattaOrdinata( ordinamenti_tratta, n_tratte, cmd, output_t);
		else if(loop)
			stampaRicercaTratta( tratta, n_tratte, output_t);
	}

	return 0;
}



/*******

Main Functions

*******/

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

void copiaTrattaInOrdinamentiTratta( Tratta **ord_t, Tratta *t, int n_tratte)
{
	for( int i = 0; i < n_tratte; i++)
		ord_t[i] = &t[i];
}

Tratta *mallocTratta( FILE *input, int *n_tratte)
{
	Tratta *t;

	fscanf( input, "%d", n_tratte);

	if( (t = (Tratta *)malloc(*n_tratte * sizeof(Tratta)) ) == NULL)
		ERR_out_of_mem();

	return t;
}

Tratta **mallocOrdinamentiTratta( int n_tratte)
{
	Tratta **ord_t;

	ord_t = (Tratta **)malloc(n_tratte * sizeof(Tratta *));
	if( ord_t == NULL)
		ERR_out_of_mem();

	return ord_t;
}

bool leggiComando( comando_e *cmd, tipo_output *output_t)
{
	/* Leggo il comando 'cmd' scritto da tastiera,
	   leggo il tipo di output 'output_t' su cui stampare */
	char prompt_line[MAX_PROMPT_LINE_CHAR] = {0};
	char *p_prompt_line = prompt_line;
	char vid_file_buff[MAX_STR] = {0};
	char word_cmd_buff[MAX_STR] = {0};
	int i = 0;


	fgets( p_prompt_line, MAX_PROMPT_LINE_CHAR, stdin);

	sscanf( p_prompt_line, "%s%n", word_cmd_buff, &i);
	p_prompt_line = p_prompt_line + i;

	sscanf( p_prompt_line, "%s", vid_file_buff);


	const char *comandi_cmd[r_fine+1] = {
		"data",
		"codice",
		"stazione_partenza",
		"stazione_arrivo",
		"ricerca_stazione_partenza",
		"apri_file",
		"fine"
	};

	bool for_completed = false;
	for( int i = 0; i <= r_fine; i++) {
		if( strcmp( word_cmd_buff, comandi_cmd[i]) == 0) {
			*cmd = i;
			for_completed = true;
			break;
		}
	}
	if( for_completed == false)
		return false;
	/* Non considero il controllo su 'vid_file_buff' */
	if( *cmd == r_open_file || *cmd == r_fine)
		return true;


	if( strcmp( vid_file_buff, "vid") == 0)
		*output_t = vid;
	else if( strcmp( vid_file_buff, "file") == 0)
		*output_t = file;
	else
		return false;

	return true;
}

FILE *openNewFile( char *src)
{
	FILE *input;

	if( (input = fopen( src, "r")) == NULL) {
		fprintf( stderr, "File: \"%s\" non trovato!\n", src);
		exit(EXIT_FAILURE);
	}

	return input;
}

void freeTratta( Tratta *t, Tratta **ord_t[n_ordinamenti])
{
	free(t);
	for( int i = 0; i < n_ordinamenti; i++)
		free(ord_t[i]);
}




/*******

Menu

*******/

void menu_ordinaTratte( Tratta **ord_t[n_ordinamenti], int n_tratte, comando_e cmd, ricerca *stato_ricerca, char *input_file_name)
{
	/* Seleziono se ordinare il vettore dei puntatori o
	   ricercare la stanzione di partenza */
	char prompt_line[MAX_PROMPT_LINE_CHAR] = {0};
	char word_buff[MAX_STR] = {0};

	switch (cmd) {
		case r_ord_data:
			bubbleSort_dataTratta( ord_t[cmd], n_tratte);
			break;
		case r_ord_codice:
			bubbleSort_codiceTratta( ord_t[cmd], n_tratte);
			break;
		case r_ord_stazione_part:
			bubbleSort_stazionePartenza( ord_t[cmd], n_tratte);
			*stato_ricerca = dicotomica;
			break;
		case r_ord_stazione_arr:
			bubbleSort_stazioneArrivo( ord_t[cmd], n_tratte);
			break;
		case r_ricerca_stazione_part:
			printf( "Inserire la stringa di ricerca: ");
			fgets( prompt_line, MAX_PROMPT_LINE_CHAR, stdin);
			sscanf( prompt_line, "%s", word_buff);

			/* Ricerca Dicotomica = O(log(n)) */
			if( *stato_ricerca == dicotomica) {
				RicercaDicotomicaTratta( ord_t[r_ord_stazione_part], word_buff, n_tratte);
			}
			else {
				RicercaLineareTratta( ord_t[r_ord_data], word_buff, n_tratte);
			}
			break;
		case r_open_file:
			printf( "Inserire il nome del file da aprire: ");
			fgets( prompt_line, MAX_PROMPT_LINE_CHAR, stdin);
			sscanf( prompt_line, "%s", input_file_name);
			*stato_ricerca = lineare;
			break;
		case r_fine:
			printf("\n-->> Adios!\n");
			break;
	}
}





/*******

  Algoritmi di ordinamento

*******/

void bubbleSort_dataTratta( Tratta **t, int n)
{
	int i = 0, j = 0;
	Tratta *temp;

	for( i = 0; i < n; i++) {
		for( j = 0; j < n-i-1; j++) {
			if( isDataTratta_gt( *t[j], *t[j+1])) {
				temp = t[j];
				t[j] = t[j+1];
				t[j+1] = temp;
			}
		}
	}
}


void bubbleSort_codiceTratta( Tratta **t, int n)
{
	int i = 0, j = 0;
	Tratta *temp;

	for( i = 0; i < n; i++) {
		for( j = 0; j < n-i-1; j++) {
			if( isCodiceTratta_gt( *t[j], *t[j+1])) {
				temp = t[j];
				t[j] = t[j+1];
				t[j+1] = temp;
			}
		}
	}
}


void bubbleSort_stazionePartenza( Tratta **t, int n)
{
	int i = 0, j = 0;
	Tratta *temp;

	for( i = 0; i < n; i++) {
		for( j = 0; j < n-i-1; j++) {
			if( isStazionePartenzaTratta_gt( *t[j], *t[j+1])) {
				temp = t[j];
				t[j] = t[j+1];
				t[j+1] = temp;
			}
		}
	}
}


void bubbleSort_stazioneArrivo( Tratta **t, int n)
{
	int i = 0, j = 0;
	Tratta *temp;

	for( i = 0; i < n; i++) {
		for( j = 0; j < n-i-1; j++) {
			if( isStazioneArrivoTratta_gt( *t[j], *t[j+1])) {
				temp = t[j];
				t[j] = t[j+1];
				t[j+1] = temp;
			}
		}
	}
}




/*********
  
  Manipolazione Tratta

*********/

int isDataTratta_gt( Tratta a, Tratta b)
{
	/* data_* sara'  2020|01|30 (yyyy|mm|gg) molto piu' facile da comparare */
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

int isCodiceTratta_gt( Tratta a, Tratta b)
{
	/* Si assume che il 'codice' sia solo 'digit' */
	int x = atoi( a.codice);
	int y = atoi( b.codice);
	if( x > y) return 1;
	return 0;
}

int isStazionePartenzaTratta_gt( Tratta a, Tratta b)
{
	if( strcmp( a.partenza, b.partenza) > 0) return 1;
	return 0;
}

int isStazioneArrivoTratta_gt( Tratta a, Tratta b)
{
	if( strcmp( a.dest, b.dest) > 0) return 1;
	return 0;
}





/******

  Stampa

******/

void stampa_nome_file( char *file_name)
{
	printf( "\n{File correntemente aperto: \"%s\"}\n", file_name);
}

void stampa_prompt( void)
{
	printf( "Inserire un comando:\n");
	printf( "data [vid/file]\n");
	printf( "codice [vid/file]\n");
	printf( "stazione_partenza [vid/file]\n");
	printf( "stazione_arrivo [vid/file]\n");
	printf( "ricerca_stazione_partenza [vid/file]\n");
	printf( "apri_file\n");
	printf( "fine\n");
	printf( "-> ");
}

void stampaTrattaOrdinata( Tratta **t[n_ordinamenti], int n_tratte, comando_e cmd, tipo_output output_t)
{
	FILE *output;
	bool is_writing_file_open = false;

	if(	output_t == vid)
		output = stdout;
	else {
		output = writingFileName();
		is_writing_file_open = true;
	}

	stampaDescrizioneOrdinamento( &output, cmd);
	for( int i = 0; i < n_tratte; i++) {
		fprintf( output, "%s %s %s %d/%d/%d %s %s %d\n",
				t[cmd][i]->codice, t[cmd][i]->partenza, t[cmd][i]->dest,
				t[cmd][i]->data.g, t[cmd][i]->data.m, t[cmd][i]->data.a,
				t[cmd][i]->ora_part, t[cmd][i]->ora_arr, t[cmd][i]->ritardo);
	}

	if( is_writing_file_open)
		fclose(output);
}

FILE *writingFileName(void)
{
	char prompt_line[MAX_PROMPT_LINE_CHAR] = {0};
	char buff[MAX_STR] = {0};

	printf( "Inserire il nome del file: ");
	fgets( prompt_line, MAX_PROMPT_LINE_CHAR, stdin);
	sscanf( prompt_line, "%s", buff);
	return fopen( buff, "a");
}

void stampaDescrizioneOrdinamento( FILE **output, comando_e cmd)
{
	const char *descrizione[4] = {
		"DATA",
		"CODICE",
		"STAZIONE DI PARTENZA",
		"STAZIONE DI ARRIVO"
	};
	fprintf( *output, "\n====> ODINAMENTO: %s <====\n", descrizione[cmd]);
}

void stampaRicercaTratta( Tratta *t, int n, tipo_output output_t)
{
	/*Stampo la tratta con il campo 'is_search' in '1' e
	 *lo riporto a '0'
	 */
	FILE *output;
	bool is_writing_file_open = false;

	if(	output_t == vid)
		output = stdout;
	else {
		output = writingFileName();
		is_writing_file_open = true;
	}

	for( int i = 0; i < n; i++) {
		if( t[i].is_search == 1) {
			fprintf( output, "%s %s %s %d/%d/%d %s %s %d\n",
					t[i].codice, t[i].partenza, t[i].dest, t[i].data.g, t[i].data.m, t[i].data.a,
					t[i].ora_part, t[i].ora_arr, t[i].ritardo);
			t[i].is_search = 0;
		}
	}

	if( is_writing_file_open)
		fclose(output);
}

void ERR_out_of_mem(void)
{
	fprintf( stderr, "ERROR: out of memory!\n");
	exit(EXIT_FAILURE);
}




/******

  Algoritmi di ricerca

******/

void RicercaDicotomicaTratta( Tratta **t, char *str, int n)
{
	/*I campi 'is_search', di 'tratta', verranno modificati in '1',
	 *di modo che la funzione 'stampaRicercaTratte'
	 *stampi solo le tratte selezionate
	 */

	int i = 0, m = 0;
	/* 'm' = elemento di mezzo della ricerca */
	m = ricercaDicotomica( t, str, 0, n);
	if( m == -1) return;
	i = m;

	/* porto i valori 'is_search' ad '1' di tutte le stringhe
	   che corrispondono al criterio di ricerca */
	while( i >= 0  &&  strncmp( str, t[i]->partenza, strlen(str)) == 0) {
		t[i]->is_search = 1;
		i--;
	}
	i = m+1;
	while( i < n  &&  strncmp( str, t[i]->partenza, strlen(str)) == 0) {
		t[i]->is_search = 1;
		i++;
	}
}

int ricercaDicotomica( Tratta **t, char *str, int l, int r)
{
	int m = (l+r)/2;
	if( l == r && strncmp( str, t[l]->partenza, strlen(str)) == 0)
		return l;
	else if( l >= r)
		return -1;
	
	if( strncmp( str, t[m]->partenza, strlen(str)) == 0) return m;

	if( strncmp( str, t[m]->partenza, strlen(str)) > 0) {
		return ricercaDicotomica( t, str, m+1, r);
	}
	return ricercaDicotomica( t, str, l, m);
}

void RicercaLineareTratta( Tratta **t, char *str, int n)
{
	/*I campi 'is_search', di 'tratta', verranno modificati in '1',
	 *di modo che la funzione 'stampaRicercaTratte'
	 *stampi solo le tratte selezionate
	 */

	int i = 0;
	for( i = 0; i < n; i++) {
		if( strncmp( str, t[i]->partenza, strlen(str)) == 0)
			t[i]->is_search = 1;
	}
}
