/* Brendon Mendicino, 271784 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR 30
#define MAX_LINES 1000

typedef enum {false, true} bool;


typedef enum {
	r_date,
	r_partenza,
	r_capolinea,
	r_ritardo,
	r_ritardo_tot,
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
	char ora_part[MAX_LINES];
	char ora_arr[MAX_LINES];
	int ritardo;
} Tratta;


void apri_file( FILE **input, char *str);
void salva_log( FILE *input, Tratta *tratta, int n_lines);
void stampa_prompt( void);
void stampa_tratta( Tratta tratta);
bool leggiComando( comando_e *cmd);
bool selezionaDati( Tratta *tratta, int n_lines, comando_e cmd);


void f_date( Tratta *tratta, int n_lines);
void f_partenza( Tratta *tratta, int n_lines);
void f_capolinea( Tratta *tratta, int n_lines);
void f_ritardo( Tratta *tratta, int n_lines);
void f_ritardo_tot( Tratta *tratta, int n_lines);


int main(void)
{
	FILE *input;
	Tratta tratta[MAX_LINES] = {{{0}}};
	int n_lines = 0;
	bool loop = true;
	comando_e cmd;
	
	apri_file( &input, "corse.txt");
	fscanf( input, "%d", &n_lines);
	salva_log( input, tratta, n_lines);

	while(loop) {
		stampa_prompt();
		if( leggiComando( &cmd) == false) {
			printf( "Opzione non valida!\n\n");
			continue;
		}

		if(cmd == r_fine) loop = false;
		
		selezionaDati( tratta, n_lines, cmd); 
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
	printf( "date [data] [data]\n");
	printf( "partenza [citta']\n");
	printf( "capolinea [citta']\n");
	printf( "ritardo [data] [data]\n");
	printf( "ritardo_tot [codice_tratta]\n");
	printf( "fine\n");
	printf( "-> ");
}


void stampa_tratta( Tratta tratta)
{
	printf( "%s %s %s %d/%d/%d %s %s %d\n",
			tratta.codice, tratta.partenza, tratta.dest,
			tratta.data.g, tratta.data.m, tratta.data.a,
			tratta.ora_part, tratta.ora_arr, tratta.ritardo);
}


void salva_log( FILE *input, Tratta *tratta, int n_lines)
{
	int i = 0;
	char data_buff[MAX_STR] = {0};

	for( i = 0; i < n_lines; i++) {
		fscanf( input, "%s%s%s%s%s%s%d",
				tratta[i].codice, tratta[i].partenza, tratta[i].dest,
				data_buff, tratta[i].ora_part, tratta[i].ora_arr,
				&tratta[i].ritardo);

		sscanf( data_buff, "%d/%d/%d",
				&tratta[i].data.g, &tratta[i].data.m, &tratta[i].data.a);
	}
}


bool leggiComando( comando_e *cmd)
{
	char word_buff[MAX_STR] = {0};
	scanf( "%s", word_buff);
	
	/* <versione alternativa>
	char *comandi_cmd[6] = {
		"date",
		"partenza",
		"capolinea",
		"ritardo",
		"ritardo_too",
		"fine"
	}
	for( int i = 0; i <= r_fine; i++) {
		if( strcmp( word_buff, comandi_cmd[i]) == 0) {
			*cmd = i;
			return true;
		}
	}
	return false;
	*/

	if( strcmp( word_buff, "date") == 0)
		*cmd = r_date;
	else if( strcmp( word_buff, "partenza") == 0)
		*cmd = r_partenza;
	else if( strcmp( word_buff, "capolinea") == 0)
		*cmd = r_capolinea;
	else if( strcmp( word_buff, "ritardo") == 0)
		*cmd = r_ritardo;
	else if( strcmp( word_buff, "ritardo_tot") == 0)
		*cmd = r_ritardo_tot;
	else if( strcmp( word_buff, "fine") == 0)
		*cmd = r_fine;
	else return false;

	return true;
}


bool selezionaDati( Tratta *tratta, int n_lines, comando_e cmd)
{
	switch (cmd) {
		case r_date: f_date( tratta, n_lines); break;
		case r_partenza: f_partenza( tratta, n_lines); break;
		case r_capolinea: f_capolinea( tratta, n_lines); break;
		case r_ritardo: f_ritardo( tratta, n_lines); break;
		case r_ritardo_tot: f_ritardo_tot( tratta, n_lines); break;
		case r_fine: return true;
	}
	return true;
}

void f_date( Tratta *tratta, int n_lines)
{
	int i = 0;
	int int_data1 = 0, int_data2 = 0, int_tratta = 0;
	Data data1, data2;
	scanf( "%d/%d/%d", &data1.g, &data1.m, &data1.a);
	scanf( "%d/%d/%d", &data2.g, &data2.m, &data2.a);

	for( i = 0; i < n_lines; i++) {
		
		int_tratta = tratta[i].data.a *10000 + tratta[i].data.m*100 + tratta[i].data.g;
		/* int_* sara'  2020|01|30  molto piu' facile da comparare */
		int_data1 = data1.a *10000 + data1.m *100 + data1.g;
		int_data2 = data2.a *10000 + data2.m *100 + data2.g;
		if( int_tratta >= int_data1 && int_tratta <= int_data2)
			stampa_tratta( tratta[i]);
	}
}


void f_partenza( Tratta *tratta, int n_lines)
{
	int i = 0;
	char word_buff[MAX_STR] = {0};
	scanf( "%s", word_buff);

	for( i = 0; i < n_lines; i++) {
		if( strcmp( word_buff, tratta[i].partenza) == 0)
			stampa_tratta( tratta[i]);
	}
}


void f_capolinea( Tratta *tratta, int n_lines)
{
	/* Stesso codice della f_partenza */
	int i = 0;
	char word_buff[MAX_STR] = {0};
	scanf( "%s", word_buff);

	for( i = 0; i < n_lines; i++) {
		if( strcmp( word_buff, tratta[i].dest) == 0)
			stampa_tratta( tratta[i]);
	}
}


void f_ritardo( Tratta *tratta, int n_lines)
{
	int i = 0;
	int int_data1 = 0, int_data2 = 0, int_tratta = 0;
	Data data1, data2;
	scanf( "%d/%d/%d", &data1.g, &data1.m, &data1.a);
	scanf( "%d/%d/%d", &data2.g, &data2.m, &data2.a);

	for( i = 0; i < n_lines; i++) {
		if( !(tratta[i].ritardo > 0)) {
			// Continua il ciclo
		}
		else {
			/* Stesso codice della f_date */
			int_tratta = tratta[i].data.a *10000 + tratta[i].data.m*100 + tratta[i].data.g;
			/* int_* sara'  2020|01|30  molto piu' facile da comparare */
			int_data1 = data1.a *10000 + data1.m *100 + data1.g;
			int_data2 = data2.a *10000 + data2.m *100 + data2.g;
			if( int_tratta >= int_data1 && int_tratta <= int_data2)
				stampa_tratta( tratta[i]);
		}
	}

}


void f_ritardo_tot( Tratta *tratta, int n_lines)
{
	int i = 0;
	char codice_buff[MAX_STR] = {0};
	int ritardo_sum = 0;
	scanf( "%s", codice_buff);

	for( i = 0; i < n_lines; i++) {
		if( strcmp( codice_buff, tratta[i].codice) == 0) {
			stampa_tratta( tratta[i]);
			ritardo_sum += tratta[i].ritardo;
		}
	}
	printf( "Ritardo totale di \"%s\": %dm\n", codice_buff, ritardo_sum);
}
