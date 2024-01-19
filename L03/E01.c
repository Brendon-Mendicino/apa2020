/* Brendon Mendicino, 271784 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_R 50
#define MAX_C 50

typedef enum {false, true} bool;


typedef struct {
	int x;
	int y;
	int base;
	int alt;
} Rettangolo;


void leggiMatrice( int M[MAX_R][MAX_C], FILE *input, int *nr, int *nc);
void stampa_mappa( int M[MAX_R][MAX_C], int nr, int nc);
bool riconosciRegione( int M[MAX_R][MAX_C], int nr, int nc, int r, int c, int *b, int *h);
void controlloRegione( Rettangolo rec[3], int r, int c, int base, int altezza);
void sost_rec( Rettangolo rec[3], int indice, int r, int c, int base, int altezza);
void stampa_max_regioni( Rettangolo rec[3]);


int main(void)
{
	FILE *input;
	int M[MAX_R][MAX_C] = {{0}};
	int nr = 0, nc = 0;
	int r = 0, c = 0;
	int b = 0, h = 0;
	bool loop = true;
	Rettangolo rec[3] = {{0}}; 
	/* rec[0] = max_altezza,
	 * rec[1] = max_base,
	 * rec[2] = max_area */
	
	if( (input = fopen( "mappa1.txt", "r") )== NULL) {
		fprintf( stderr, "File non trovato!\n");
		exit(1);
	}
	leggiMatrice( M, input, &nr, &nc);
	stampa_mappa( M, nr, nc);

	while( loop) {
		printf( "Inserire le coordinate del rettangolo [riga] [colonna]: ");

		if( scanf( "%d%d", &r, &c) != 2) loop = false;
		else if( riconosciRegione( M, nr, nc, r, c, &b, &h)) {
			printf( "Rettangolo con vertice in:\n");
			printf( "<%d,%d> base: %d, altezza: %d\n", r, c, b, h);
			controlloRegione( rec, r, c, b, h);
		}
		else {
			printf( "Rettangolo non trovato o (r,c)  non e' il vertice sinistro\n");
			printf( "Riprova!\n\n");
		}

	}
	stampa_max_regioni( rec);

	fclose(input);

	return 0;
}


void leggiMatrice( int M[MAX_R][MAX_C], FILE *input, int *nr, int *nc)
{
	fscanf( input, "%d%d", nr, nc);
	for( int i = 0; i < *nr; i++) {
		for( int j = 0; j < *nc; j++) {
			fscanf( input, "%d", &M[i][j]);
		}
	}
}


void stampa_mappa( int M[MAX_R][MAX_C], int nr, int nc)
{
	for( int i = 0; i < nr; i++) {
		for( int j = 0; j < nc; j++) {
			printf( "%c", (M[i][j] == 0)? '~': '#');
		}
		printf("\n");
	}
	printf("\n");
}



bool riconosciRegione( int M[MAX_R][MAX_C], int nr, int nc, int r, int c, int *b, int *h)
{
	int base = 0, altezza = 0;
	
	if( M[r][c] == 0) return false;

	/* 'r' e 'c' non si trovano ad un vertice */
	if( r > 0 && M[r-1][c] == 1) return false;
	if( c > 0 && M[r][c-1] == 1) return false;

	while( M[r][c+base] == 1 && c+base < nc) base++;
	while( M[r+altezza][c] == 1 && r+altezza < nr) altezza++;

	*b = base;
	*h = altezza;

	return true;
}



void controlloRegione( Rettangolo rec[3], int r, int c, int base, int altezza)
{
	/* controllo se la regione e' una max h, b, A */
	if( altezza > rec[0].alt) {
		sost_rec( rec, 0, r, c, altezza, base);
	}
	if( base > rec[1].base) {
		sost_rec( rec, 1, r, c, altezza, base);
	}
	if( base * altezza > rec[2].base * rec[2].alt) {
		sost_rec( rec, 2, r, c, altezza, base);
	}
}



void sost_rec( Rettangolo rec[3], int indice, int r, int c, int base, int altezza)
{
	/* rec[0] = max_altezza,
	 * rec[1] = max_base,
	 * rec[2] = max_area */
	rec[indice].x = r;
	rec[indice].y = c;
	rec[indice].alt = altezza;
	rec[indice].base = base;
}


void stampa_max_regioni( Rettangolo rec[3])
{
	/* stampo le regioni con h, b, A, massime */
	for( int i = 0; i < 3; i++) {
		switch (i) {
			case 0: printf("Max Altezza: "); break;
			case 1: printf("Max Base: "); break;
			case 2: printf("Max Area: "); break;
		}
		printf("coord. <%d,%d> b=%d, h=%d, A=%d\n",
				rec[i].x, rec[i].y, rec[i].base, rec[i].alt, rec[i].base * rec[i].alt);
	}
}
