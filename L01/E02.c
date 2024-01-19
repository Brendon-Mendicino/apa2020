/* Brendon Mendicino, 271784 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR 200
#define MAX_WORD 30
#define S 30

typedef enum {false, true} bool;

typedef struct {
	char ricodifica[MAX_WORD];
	char originale[MAX_WORD];
} Dizionario;


void apri_file( FILE **fin, char *str);
int controllo( char *word, Dizionario *d, int n);


int main(void)
{
	FILE *input_sor, *input_diz, *output;
	Dizionario dizionario[S] = {0};
	char line_buff[MAX_CHAR] = {0};
	char word_buff[MAX_WORD] = {0};
	int n_ricodifica = 0;
	
	apri_file( &input_sor, "sorgente.txt");
	apri_file( &input_diz, "dizionario.txt");
	output = fopen( "ricodificato.txt", "w");

	/* leggo "dizionario.txt" ed inserisco i dati in dizionario[] */
	fscanf( input_diz, "%d", &n_ricodifica);
	for( int i = 0; i < n_ricodifica; i++) {
		fscanf( input_diz, "%s%s", dizionario[i].ricodifica, dizionario[i].originale );
	}
	
	int x = 0, y = 0;
	while( fgets( line_buff, MAX_CHAR, input_sor) != NULL) {
		/* costruisco una parola e poi la controllo */
		for( y = 0; ( (word_buff[x] = line_buff[y]) != '\0'); y++) {
			if( word_buff[x] == ' ' || word_buff[x] == '\n') {
				bool is_space = false;
				if( word_buff[x] == ' ') is_space = true;
				word_buff[x] = '\0';
				controllo( word_buff, dizionario, n_ricodifica);
				fprintf( output, "%s", word_buff);
				if( is_space == true) fprintf( output, " ");
				x = -1; /* resetto x (-1 perhe' dopo e' presente un x++) */
			}
			x++;
		}
		fprintf( output, "\n");
	}

	fclose(input_diz);
	fclose(input_sor);
	fclose(output);

	return 0;
}


void apri_file( FILE **fin, char *str)
{
	if( (*fin = fopen( str, "r") ) == NULL) {
		fprintf( stderr, "File: \"%s\" non trovato!\n", str);
		exit(1);
	}
}

int controllo( char *word, Dizionario *d, int n)
{
	int len = strlen( word);

	for( int i = 0; i < n; i++) {
		for( int j = 0; j <= len - (int)strlen( d[i].originale); j++) {
			if( strncmp( &word[j], d[i].originale, strlen( d[i].originale)) == 0) {
				strcpy( &word[j], d[i].ricodifica);
				return 1;
			}
		}
	}
	return 0;
}
