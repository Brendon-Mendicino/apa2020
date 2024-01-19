
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STR 25
#define MAX_SEQ 5
#define MAX_OCCUR 10
#define MAX_R_SEQ 20
#define MAX_CHAR_INLINE 200

typedef enum {false, true} bool;


typedef struct {
	char seq[MAX_SEQ];
	/* il numero di sequenze presenti nella struct, max=10 */
	int n_seq;
	int coord[MAX_OCCUR][2];
	char word[MAX_OCCUR][MAX_STR];
} Sequenza;


void apri_file( FILE **input, char *str);
void elabora_line( Sequenza *s, int n_lines_s, int riga, char *line);
bool is_word_present( Sequenza s, char *word_buff);
bool my_sscanf( char *line, char *word_buff);
void stampa_risultati( Sequenza *s, int n_lines_s);


int main( void)
{
	FILE *in_s, *in_t;
	int n_lines_s = 0;
	char line_buffer[MAX_CHAR_INLINE] = {0};
	Sequenza sequenza[MAX_R_SEQ] = {{{0}}};

	apri_file( &in_s, "sequenze.txt");
	apri_file( &in_t, "testo.txt");

	fscanf( in_s, "%d", &n_lines_s);
	for( int i = 0; i < n_lines_s; i++)
		fscanf( in_s, "%s", sequenza[i].seq);

	int riga = 0;
	while( fgets( line_buffer, MAX_CHAR_INLINE, in_t) != NULL) {
		elabora_line( sequenza, n_lines_s, riga, line_buffer);
		riga++;
	}

	stampa_risultati( sequenza, n_lines_s);


	fclose(in_s);
	fclose(in_t);

	return 0;
}


void apri_file( FILE **input, char *str)
{
	if( (*input = fopen( str, "r")) == NULL) {
			fprintf( stderr, "File: \"%s\" non trovato!\n", str);
			exit(1);
	}
}

void elabora_line( Sequenza *s, int n_lines_s, int riga, char *line)
{
	char word_buff[MAX_STR] = {0};
	int colonna = 0;

	while( my_sscanf( &line[colonna], word_buff) == true) {
		for( int i = 0; i < n_lines_s; i++) {
			if( s[i].n_seq == 10) {
				// continua il ciclo
			}
			else if( is_word_present( s[i], word_buff) == true) {
				s[i].coord[s[i].n_seq][0] = riga+1;
				s[i].coord[s[i].n_seq][1] = colonna+1;
				strcpy( s[i].word[s[i].n_seq], word_buff);
				s[i].n_seq += 1;
			}
		}	
		colonna += strlen( word_buff);
		while( isspace(line[colonna]) ) colonna++;
		while( ispunct(line[colonna]) ) colonna++;
	}
}


bool my_sscanf( char *line, char *word_buff)
{
	int i = 0;
	while( isalnum( line[i])) {
		word_buff[i] = line[i];
		i++;
	}
	if( i == 0) return false;

	word_buff[i] = '\0';
	return true;
}


bool is_word_present( Sequenza s, char *word_buff)
{
	/* copio i dati */
	char a = 0;
	char word_cpy[MAX_STR] = {0};
	strcpy( word_cpy, word_buff);

	/* porto entrambe le stringhe in 'lowercases' */
	for(int i = 0; i < strlen(word_cpy); i++) {
		if( isupper( word_cpy[i])) {
			a = (char)tolower( word_cpy[i]);
			word_cpy[i] = a;
		}
	}
	for(int i = 0; i < strlen(s.seq); i++) {
		if( isupper( s.seq[i])) {
			a = (char)tolower( s.seq[i]);
			s.seq[i] = a;
		}
	}

	if( strstr( word_cpy, s.seq) != NULL) return true;

	return false;
}


void stampa_risultati( Sequenza *s, int n_lines_s)
{
	for( int i = 0; i < n_lines_s; i++) {
		printf( "La sequenza \"%s\" e' contenuta in <riga,colonna>:\n", s[i].seq);
		for(int j = 0; j < s[i].n_seq; j++) {
			printf( "%s <%d,%d>, ", s[i].word[j], s[i].coord[j][0], s[i].coord[j][1]);
		}
		printf("\n\n");
	}

}
