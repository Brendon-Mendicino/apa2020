/* Brendon Mendicino, 271784 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME_FILE 20
#define MAX_COLONNE 30
#define MAX_RIGHE 30
#define MAX_RIGA 100

typedef struct {
	char selettore[MAX_RIGA];
	int indice;
	char direzione[MAX_RIGA];
	int posizioni;
	int height;
	int width;
} In;

int controllo( In in);

void stampa( int matrice[MAX_RIGHE][MAX_COLONNE], In in);
void trasf_riga( int matrice[MAX_RIGHE][MAX_COLONNE], In in);
void trasf_colonna( int matrice[MAX_RIGHE][MAX_COLONNE], In in);

int main(void)
{
	FILE *input;
	int matrice[MAX_RIGHE][MAX_COLONNE] = {{0}};
	char nome_file[MAX_NOME_FILE] = {0};
	char riga[MAX_RIGA] = {0};
	In in = {0};

	printf( "Inserire il nome del file: ");
	scanf( "%s", nome_file);

	if( (input = fopen( nome_file, "r")) == NULL) {
		fprintf( stderr, "File non trovato!\n");
		exit(1);
	}
	fscanf( input, "%d%d", &in.height, &in.width);

	for( int i = 0; i < in.height; i++) {
		for( int j = 0; j < in.width; j++) {
			fscanf( input, "%d", &matrice[i][j]);
		}
	}
	stampa( matrice, in);

	do {
		printf( "Inserirsci riga [riga/colonna/fine][indice][sx/dx//su/giu][posizione]:\n");
		fgets( riga, MAX_RIGA, stdin);
		// Svuota il buffer da tastiera
		if( riga[0] == '\n') fgets( riga, MAX_RIGA, stdin);
		sscanf( riga, "%s%d%s%d", in.selettore, &in.indice, in.direzione, &in.posizioni);
		if( controllo( in) != 0) continue;

		if( strcmp( in.selettore, "riga") == 0) {
			trasf_riga( matrice, in);
		}
		else if( strcmp( in.selettore, "colonna") == 0) {
			trasf_colonna( matrice, in);
		}

		stampa( matrice, in);
	} while( strcmp( in.selettore, "fine") != 0);


	fclose(input);
	return 0;
}

int controllo( In in)
{
	// TODO: da finire il controllo
	/*
	if( strcmp( in.selettore, "riga") == 0) {
		if( in.indice


	*/
	return 0;
}

void trasf_riga( int matrice[MAX_RIGHE][MAX_COLONNE], In in)
{
	int matrice_buff[MAX_RIGHE] = {0};
	for( int i = 0; i < in.width; i++) {
		matrice_buff[i] = matrice[in.indice][i];
	}

	if( strcmp( in.direzione, "sx") == 0) {
		for( int i = 0; i < in.width -in.posizioni; i++) {
			 matrice[in.indice][i +in.posizioni] = matrice_buff[i];
		}
		for( int i = 0; i < in.posizioni; i++) {
			matrice[in.indice][i] = matrice_buff[i + in.width-in.posizioni];
		}
	}
	else if( strcmp( in.direzione, "dx") == 0) {
		for( int i = 0; i < in.width -in.posizioni; i++) {
			matrice[in.indice][i] = matrice[in.indice][i +in.posizioni];
		}
		for( int i = 0; i < in.posizioni; i++) {
			matrice[in.indice][i + in.width-in.posizioni] = matrice_buff[i];
		}
	}
}

// Simile a trans_riga con widht sostiuto da height, con caselle scambiate inveritte
void trasf_colonna( int matrice[MAX_RIGHE][MAX_COLONNE], In in)
{
	int matrice_buff[MAX_COLONNE] = {0};
	for( int i = 0; i < in.height; i++) {
		matrice_buff[i] = matrice[i][in.indice];
	}
	if( strcmp( in.direzione, "giu") == 0) {
		for( int i = 0; i < in.height -in.posizioni; i++) {
			 matrice[i +in.posizioni][in.indice] = matrice_buff[i];
		}
		for( int i = 0; i < in.posizioni; i++) {
			matrice[i][in.indice] = matrice_buff[i + in.height-in.posizioni];
		}
	}
	else if( strcmp( in.direzione, "su") == 0) {
		for( int i = 0; i < in.height -in.posizioni; i++) {
			matrice[i][in.indice] = matrice[i +in.posizioni][in.indice];
		}
		for( int i = 0; i < in.posizioni; i++) {
			matrice[i + in.height-in.posizioni][in.indice] = matrice_buff[i];
		}
	}
}



void stampa( int matrice[MAX_RIGHE][MAX_COLONNE], In in)
{
	for( int i = 0; i < in.height; i++) {
		putchar('[');
		for( int j = 0; j < in.width; j++) {
			printf( "%d ", matrice[i][j]);
		}
		putchar(']');
		putchar('\n');
	}
	putchar('\n');
}
