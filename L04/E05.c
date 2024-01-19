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


/* Main Functions */
FILE *apri_file( char *str);
void salva_log( FILE *input, Tratta *tratta, int n_tratte);
void copiaTrattaInOrdinamentiTratta( Tratta *t, Tratta *ord_t[][MAX_CORSE], int n_ord, int n);
bool leggiComando( comando_e *cmd, tipo_output *output_t);

/* Menu */
void menu_ordinaTratte( Tratta *ord_t[][MAX_CORSE], int n_tratte, comando_e cmd, ricerca *stato_ricerca);

/* Algoritmi di Sorting */
void bubbleSort_dataTratta( Tratta **t, int n);
void bubbleSort_codiceTratta( Tratta **t, int n);
void bubbleSort_stazionePartenza( Tratta **t, int n);
void bubbleSort_stazioneArrivo( Tratta **t, int n);

/* Manipolazione Tratta */
int TrattaData_gt( Tratta a, Tratta b);
int TrattaCodice_gt( Tratta a, Tratta b);
int TrattaStazionePartenza_gt( Tratta a, Tratta b);
int TrattaStazioneArrivo_gt( Tratta a, Tratta b);

/* Algoritmi di Ricerca */
void RicercaDicotomicaTratta( Tratta **tratta, char *str, int n_tratte);
void RicercaLineareTratta( Tratta **tratta, char *str, int n_tratte);
int  ricercaDicotomica( Tratta **tratta, char *str, int l, int r);

/* Stampa */
void stampa_prompt( void);
void stampaTratta( Tratta *t[][MAX_CORSE], int n_tratte, comando_e cmd, tipo_output output_t);
void stampaDescrizioneOrdinamento( FILE **output, comando_e cmd);
void stampaRicercaTratta( Tratta *t, int n, tipo_output output_t);



int main(void)
{
	FILE *input;
	// r_fine-2 == numero di ordinamenti della tratte
	int n_ordinamenti = r_fine-1;
	Tratta tratta[MAX_CORSE] = {{{0}}};
	Tratta *ordinamenti_tratta[n_ordinamenti][MAX_CORSE];

	bool loop = true;
	int	n_tratte = 0;
	ricerca      stato_ricerca = lineare;
	comando_e    cmd = r_ord_data;
	tipo_output  output_t;
	

	input = apri_file( "corse.txt");
	fscanf( input, "%d", &n_tratte);
	salva_log( input, tratta, n_tratte);

	copiaTrattaInOrdinamentiTratta( tratta, ordinamenti_tratta, n_ordinamenti, n_tratte);

	while(loop) {
		stampa_prompt();
		if( leggiComando( &cmd, &output_t) == false) {
			fprintf( stderr, "Opzione non valida!\n\n");
			continue;
		}
		if( cmd == r_fine) loop = false;

		menu_ordinaTratte( ordinamenti_tratta, n_tratte, cmd, &stato_ricerca);

		/* Uso 'ord_t' per la stampa normale, mentre per la stampaRicercaTratta
		   uso 'tratta', dato che i puntatori a 'tratta' avranno modificato
		   il campo 'is_search' in '1' dei vettori corrsipondeti al criterio di
		   ricerca, verranno riportati a '0' all'interno della funzione */
		if( cmd != r_ricerca_stazione_part && loop)
			stampaTratta( ordinamenti_tratta, n_tratte, cmd, output_t);
		else if(loop)
			stampaRicercaTratta( tratta, n_tratte, output_t);
	}


	fclose(input);
	return 0;
}



/*******

  Main Functions

*******/

FILE *apri_file( char *str)
{
	FILE *input;
	if( (input = fopen( str, "r")) == NULL) {
		fprintf( stderr, "File: \"%s\" non trovato!\n", str);
		exit(1);
	}
	return input;
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

void copiaTrattaInOrdinamentiTratta( Tratta *t, Tratta *ord_t[][MAX_CORSE], int n_ord, int n)
{
	for( int i = 0; i < n_ord; i++) {
		for( int j = 0; j < n; j++) {
			ord_t[i][j] = &t[j];
		}
	}
}

bool leggiComando( comando_e *cmd, tipo_output *output_t)
{
	/* Leggo il comando 'cmd' scritto da tastiera,
	   leggo il tipo di output 'output_t' su cui stampare */
	char vid_file_buff[MAX_STR] = {0};
	char word_cmd_buff[MAX_STR] = {0};

	scanf( "%s", word_cmd_buff);
	if( strcmp( word_cmd_buff, "fine") == 0) {
		*cmd = r_fine;
		return true;
	}
	scanf( "%s", vid_file_buff);

	const char *comandi_cmd[5] = {
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



/*******

  Menu

*******/

void menu_ordinaTratte( Tratta *ord_t[][MAX_CORSE], int n_tratte, comando_e cmd, ricerca *stato_ricerca)
{
	/* Seleziono se ordinare il vettore dei puntatori o
	   ricercare la stanzione di partenza */
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
			/* I campi 'is_search', di 'tratta', verranno modificati in '1',
			   di modo che la funzione 'stampaRicercaTratte'
			   stampi solo le tratte selezionate */
			printf( "Inserire il campo di ricerca: ");
			scanf( "%s", word_buff);
			/* Se lo stato e' dicotomico vuol dire che posso accedere
			   al vettore ordinato tutte le volte che chiamo la funzione,
			   essendo la ricerca dicotomica la piu efficente O(log(n)) */
			if( *stato_ricerca == dicotomica) {
				RicercaDicotomicaTratta( ord_t[r_ord_stazione_part], word_buff, n_tratte);
			}
			else {
				RicercaLineareTratta( ord_t[r_ord_data], word_buff, n_tratte);
			}
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
			if( TrattaData_gt( *t[j], *t[j+1])) {
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
			if( TrattaCodice_gt( *t[j], *t[j+1])) {
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
			if( TrattaStazionePartenza_gt( *t[j], *t[j+1])) {
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
			if( TrattaStazioneArrivo_gt( *t[j], *t[j+1])) {
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





/******

  Stampa

******/

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

void stampaTratta( Tratta *t[][MAX_CORSE], int n_tratte, comando_e cmd, tipo_output output_t)
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

	stampaDescrizioneOrdinamento( &output, cmd);
	for( int i = 0; i < n_tratte; i++) {
		fprintf( output, "%s %s %s %d/%d/%d %s %s %d\n",
				t[cmd][i]->codice, t[cmd][i]->partenza, t[cmd][i]->dest,
				t[cmd][i]->data.g, t[cmd][i]->data.m, t[cmd][i]->data.a,
				t[cmd][i]->ora_part, t[cmd][i]->ora_arr, t[cmd][i]->ritardo);
	}

	if( is_file_open)
		fclose(output);
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




/******

  Algoritmi di ricerca

******/

void RicercaDicotomicaTratta( Tratta **t, char *str, int n)
{
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
	int i = 0;
	for( i = 0; i < n; i++) {
		if( strncmp( str, t[i]->partenza, strlen(str)) == 0)
			t[i]->is_search = 1;
	}
}
