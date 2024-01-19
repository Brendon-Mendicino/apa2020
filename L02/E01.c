/* Brendon Mendicino, 271784 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_R 50
#define MAX_C 50

typedef struct {
	int x;
	int y;
	int base;
	int alt;
} Rettangolo;


void salva_mappa( int mappa[MAX_R][MAX_C], FILE *input, int *nr, int *nc);
void stampa_mappa( int mappa[MAX_R][MAX_C], int nr, int nc);
void finder( Rettangolo rec[3], int mappa[MAX_R][MAX_C], int nr, int nc, int x, int y);
void sost_rec( Rettangolo rec[3], int indice, int x, int y, int alt, int base);


int main(void)
{
	FILE *input;
	int mappa[MAX_R][MAX_C] = {{0}};
	int nr = 0, nc = 0;
	Rettangolo rec[3] = {{0}}; 
	/* rec[0] = max_altezza,
	 * rec[1] = max_base,
	 * rec[2] = max_area */
	
	if( (input = fopen( "mappa1.txt", "r") )== NULL) {
		fprintf( stderr, "File non trovato!\n");
		exit(1);
	}
	salva_mappa( mappa, input, &nr, &nc);
	stampa_mappa( mappa, nr, nc);

	for( int x = 0; x < nr; x++) {
		for( int y = 0; y < nc; y++) {
			if(mappa[x][y] == 1) {
				finder( rec, mappa, nr, nc, x, y);
			}
		}
	}

	/* stampo la max h, b, A */
	for( int i = 0; i < 3; i++) {
		switch (i) {
			case 0: printf("Max Altezza: "); break;
			case 1: printf("Max Base: "); break;
			case 2: printf("Max Area: "); break;
		}
		printf("coord. <%d,%d> b=%d, h=%d, A=%d\n",
				rec[i].x, rec[i].y, rec[i].base, rec[i].alt, rec[i].base * rec[i].alt);
	}

	fclose(input);

	return 0;
}


void salva_mappa( int mappa[MAX_R][MAX_C], FILE *input, int *nr, int *nc)
{
	fscanf( input, "%d%d", nr, nc);
	for( int i = 0; i < *nr; i++) {
		for( int j = 0; j < *nc; j++) {
			fscanf( input, "%d", &mappa[i][j]);
		}
	}
}


void stampa_mappa( int mappa[MAX_R][MAX_C], int nr, int nc)
{
	for( int i = 0; i < nr; i++) {
		for( int j = 0; j < nc; j++) {
			printf( "%c", (mappa[i][j] == 0)? '~': '#');
		}
		printf("\n");
	}
	printf("\n");
}



void finder( Rettangolo rec[3], int mappa[MAX_R][MAX_C], int nr, int nc, int x, int y)
{
	int alt = 0, base = 0;
	while( mappa[x][y+base] == 1 && y+base < nr) base++;
	while( mappa[x+alt][y] == 1 && x+alt < nc) alt++;
	
	if( alt > rec[0].alt) {
		sost_rec( rec, 0, x, y, alt, base);
	}
	if( base > rec[1].base) {
		sost_rec( rec, 1, x, y, alt, base);
	}
	if( base * alt > rec[2].base * rec[2].alt) {
		sost_rec( rec, 2, x, y, alt, base);
	}

	/* Porto il rettangolo sulla mappa col valore 2,
	   evitando un riconoscimento nel main di un rettangolo
	   gia analizzato */
	for( int i = x; i < base; i++) {
		for( int j = y; j < alt; j++) {
			mappa[i][j] = 2;
		}
	}
}


void sost_rec( Rettangolo rec[3], int indice, int x, int y, int alt, int base)
{
	/* rec[0] = max_altezza,
	 * rec[1] = max_base,
	 * rec[2] = max_area */
	rec[indice].x = x;
	rec[indice].y = y;
	rec[indice].alt = alt;
	rec[indice].base = base;
}
