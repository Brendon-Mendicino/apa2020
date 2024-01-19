/* Brendon Mendicino, 271784 */


#include <stdio.h>
#include <stdlib.h>

#define maxR 20
#define maxC 20

typedef struct {
	int score;
} Squadra;

typedef struct {
	int max_score;
	int top;
} Punteggio;

int main(void)
{
	FILE *input;
	int matrice[maxR][maxC] = {{0}};
	int righe = 0, colonne = 0;
	char matrice_buff = 0;
	Squadra s[maxR] = {0};
	Punteggio punteggio = {0};

	if( (input = fopen( "campionato.txt", "r") ) == NULL) {
		fprintf( stderr, "File non trovato!\n");
		exit(1);
	}
	// inserisco tutti i dati nella matrice
	int i = 0, j = 0, max_j = 0;
	while( (matrice_buff = fgetc( input)) != EOF) {
		if( matrice_buff == '\n') {
			j = 0;
			i++;
		}
		else if( matrice_buff != ' ') {
			matrice[i][j] = matrice_buff - '0';
			j++;
		}
		if( j > max_j) max_j = j;
	}
	righe = i;
	colonne = max_j;
	for( int x = 0; x < righe; x++) {
		for( int y = 0; y < colonne; y++) {
			printf( "%d ", matrice[x][y]);
		}
		putchar('\n');
	}

	for( int giornata = 0; giornata < colonne; giornata++) {
		for( int squadra = 0; squadra < righe; squadra++) {
			s[squadra].score += matrice[squadra][giornata];
			if( s[squadra].score > punteggio.max_score) {
				punteggio.max_score = s[squadra].score;
				punteggio.top = squadra;
			}
		}

		printf( "Squadra \"%d\" in cima alla lista con \"%d\" punti! (giornata %d)\n", punteggio.top, punteggio.max_score, giornata + 1);
	}



	fclose(input);
	return 0;
}
