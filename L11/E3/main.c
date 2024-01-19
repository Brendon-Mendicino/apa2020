#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"


static void memCheck( void *p)
{
	if( p == NULL) {
		fprintf( stderr, "ERROR: out of memory!\n");
		exit(EXIT_FAILURE);
		}
}


int partition( char **nome, int l, int r)
{
	int i = l - 1;
	int j = r;
	static char pivot[STR_LEN];
	static char temp[STR_LEN];
	strcpy( pivot, nome[r]);

#	define swap( A, B)				\
	strcpy( temp, nome[(A)]);		\
	strcpy( nome[(A)], nome[(B)]);	\
	strcpy( nome[(B)], temp);

	for( ; ; ) {
		while( strcmp( nome[++i], pivot) < 0);
		while( strcmp( nome[--j], pivot) > 0)
			if( j == l)
				break;
		if( i >= j)
			break;
		swap( i, j);
	}
	swap( i, r);

#	undef swap
	return i;
}

void quicksort( char **nome, int l, int r)
{
	int m;
	if( l >= r)
		return;

	m = partition( nome, l, r);
	quicksort( nome, l, m-1);
	quicksort( nome, m+1, r);
}


void stampaVerticiOrdinati( Graph G)
{
	int V;
	int i, j;
	int temp;
	int *ver_list;
	char **nome;
	char **nome_incid;

	V = GRAPHgetVert( G);

	// Inizializzazione
	ver_list = (int *)malloc( V * sizeof(int));
	memCheck( ver_list);
	nome = (char **)malloc( V * sizeof(char *));
	memCheck( nome);
	for( i = 0; i < V; i++) {
		nome[i] = (char *)malloc( STR_LEN * sizeof(char));
		memCheck( nome[i]);
	}
	nome_incid = (char **)malloc( V * sizeof(char *));
	memCheck( nome_incid);
	for( i = 0; i < V; i++) {
		nome_incid[i] = (char *)malloc( STR_LEN * sizeof(char));
		memCheck( nome_incid[i]);
	}

	// Lista dei vertici
	GRAPHgetVertList( G, ver_list);
	for( i = 0; i < V; i++)
		GRAPHgetLableByIndex( G, ver_list[i], nome[i]);
	quicksort( nome, 0, V-1);

	printf( "Lista dei vertici:\n");
	for( i = 0; i < V; i++)
		printf( "%3d) %s\n", i+1, nome[i]);

	// Lista dei vertici incidenti
	for( i = 0; i < V; i++) {
		temp = GRAPHgetVertIncidenti( G, GRAPHgetIndexByLable( G, nome[i]), ver_list);
		for( j = 0; j < temp; j++)
			GRAPHgetLableByIndex( G, ver_list[j], nome_incid[j]);
		quicksort( nome_incid, 0, temp-1);

		printf( "Vertici insistenti su \"%s\":\n", nome[i]);
		for( j = 0; j < temp; j++)
			printf( "%s\n", nome_incid[j]);
	}
	printf( "\n");

	free( ver_list);
	for( i = 0; i < V; i++)
		free( nome[i]);
	free( nome);
	for( i = 0; i < V; i++)
		free( nome_incid[i]);
	free( nome_incid);
}


void sottografoCompleto( Graph G)
{
	char *buff;
	int *list;
	int mode;

	buff = malloc( STR_LEN * sizeof(char));
	memCheck(buff);
	list = (int *)malloc( 3 * sizeof(int));
	memCheck(list);

	printf( "Controlla se 3 vertici formano un sotto grafo connesso.\n");
	printf( "Inserisci 3 vertici per nome:\n");
	for( int i = 0; i < 3; i++) {
		printf( "%d: ", i+1);
		scanf( "%s", buff);
		list[i] = GRAPHgetIndexByLable( G, buff);
	}

	printf( "Scegliere la modalita':\n");
	printf( " 1) Matrice di adiacenza;  2)Lista di adiacenza\n");
	printf( ">>> ");

	scanf( "%d", &mode);
	if( mode == 1)
		mode = MODE_MATR;
	else if( mode == 2)
		mode = MODE_LIST;
	else {
		fprintf( stderr, "Invalid input!\n");
		return;
	}

	if( GRAPHisComplete( G, list, 3, mode))
		printf( "I 3 vertici formano un sottografo completo!\n");
	else
		printf( "I 3 vertici non formano un sottografo completo!\n");
	printf( "\n");

	free( buff);
	free( list);
}

void print_prompt(void)
{
	printf( " 1) ~> Elenca in ordine alfabetico i vertici e quelli a lore insistenti.\n");
	printf( " 2) ~> Dati 3 vertici, controlla se sono un sottografo completo.\n");
	printf( " 3) ~> Genera lista delle adiacenze.\n");
	printf( " 4) Exit.\n");
	printf( ">>> ");
	fflush( stdout);
}

int read_command( int *cmd)
{
	scanf( "%d", cmd);
	if( *cmd <= 0 || *cmd > 4)
		return 0;
	return 1;
}

int main( void)
{
	Graph G;
	int loop = 1;
	int cmd;

	G = GRPAHload( "grafo.txt");
	GRAPHprint( stdout, G);

	while(loop) {
		print_prompt();
		if( !read_command( &cmd)) {
			fprintf( stderr, "Input invalido!\n");
			continue;
		}
		if( cmd == 1)
		{
			stampaVerticiOrdinati( G);
		}
		else if( cmd == 2)
		{
			sottografoCompleto( G);
		}
		else if( cmd == 3)
		{
			GRAPHgenerateAdjList( G);
		}
		else if( cmd == 4)
		{
			loop = 0;
		}
	}

	GRAPHfree( G);
	return 0;
}
