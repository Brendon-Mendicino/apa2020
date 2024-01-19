
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "graph.h"




static void mem_check( void *p)
{
	if( p == NULL) {
		fprintf( stderr, "ERROR: out of memory!\n");
		exit(EXIT_FAILURE);
	}
}

int partition( edge_t *a, int l, int r)
{
	static edge_t temp;
	int i = l-1;
	int j = r;
	edge_t pivot = a[r];
#	define swap(A, B)	\
	temp   = a[(A)];	\
	a[(A)] = a[(B)];	\
	a[(B)] = temp;
	for( ; ; ) {
		while( a[++i].v < pivot.v);
		while( a[--j].v > pivot.v)
			if( j == l)
				break;
		if( i >= l)
			break;
		swap( i, j);
	}
	swap( i, r);
#	undef swap
	return i;
}

void sort( edge_t *a, int l, int r)
{
	int m;
	if( l >= r)
		return;
	m = partition( a, l, r);
	sort( a, l, m-1);
	sort( a, m+1, r);
}

void quicksort( edge_t *a, int len)
{
	sort( a, 0, len-1);
}

// Fonde i due vettori eliminando le copie
// Il nuovo vettore e' e1
int merge_edges( edge_t *e1, int len1, edge_t *e2, int len2)
{
	int i, j;
	int is_duplicate;
	int new_len = len1;

	for( i = 0; i < len2; i++) {
		is_duplicate = 0;
		for( j = 0; j < len1; j++) {
			if( e1[j].v == e2[i].v && e1[j].v == e2[i].w) {
				is_duplicate = 1;
				break;
			}
		}
		// Posso mente aumentare le celle dato che sono tutt
		// inizializzati al massimo
		if( !is_duplicate) {
			e1[new_len++] = e2[i];
		}
	}
	return new_len;
}




void add_edges_to_sol( edge_t **f_sol, edge_t *edges, int *n_sol, int card_min)
{
	int i, j;
	int valid_edges = 0;

	quicksort( edges, card_min);
	// Controllo se la soluzione non e' gia presente;
	// se lo e' allora esco dalla funzione, altrimenti la aggiungo
	for( i = 0; i < *n_sol; i++) {
		valid_edges = 0;
		// Posso controllare perche' i vettori sono ordinati
		for( j = 0; j < card_min; j++) {
			if( !(f_sol[i][j].v == edges[j].v && f_sol[i][j].w == edges[j].w)) {
				valid_edges = 1;
				break;
			}
		}
		if( !valid_edges)
			return;
	}
	// Alloco la memoria per f_sol[i]
	f_sol[i] = (edge_t *)malloc( card_min * sizeof(edge_t));
	mem_check( f_sol[i]);

	merge_edges( f_sol[i], 0, edges, card_min);
	(*n_sol)++;
}


void find_all_sets( Graph G, edge_t *edges, int card_min)
{
	int i = 0, j = 0;

	int n_sol = 0;
	edge_t **f_sol;

	int max_set = 0;
	int max_val = 0;

	f_sol = (edge_t **)malloc( GRAPHget_edges(G)*sizeof(edge_t *));
	mem_check( f_sol);

	// Trovo tutti i possibili insiemi di archi Back di cardinalita' minima
	for( int v = 0; v < GRAPHget_vert(G); v++) {
		if( GRAPH_dfs_find_back_edges( G, edges, v) == card_min) {
			add_edges_to_sol( f_sol, edges, &n_sol, card_min);
		}
	}

	// Stampa
	printf( "\nInsieme degli archi.\n");
	printf( "Cardinalita' minima: %d\n", card_min);
	for( int j = 0; j < n_sol; j++) {
		printf( "{");
		for( int k = 0; k < card_min; k++) {
			printf( "(%d, %d)%s", f_sol[j][k].v, f_sol[j][k].w, k==card_min-1? "":", ");
		}
		printf( "}\n");
	}


	// 2.
	// Trovare l'insieme con peso maggiore
	for( i = 0; i < n_sol; i++) {
		int curr_value = 0;
		for( j = 0; j < card_min; j++) {
			curr_value += f_sol[i][j].wt;
		}
		if( curr_value > max_val) {
			max_val = curr_value;
			max_set = i;
		}
	}
 
	// Stampa
	printf( "\nMax value: %d\n", max_val);
	printf( "{");
	for( i = 0; i < card_min; i++) 
		printf( "(%d, %d)%s", f_sol[max_set][i].v, f_sol[max_set][i].w, i==card_min-1? "":", ");
	printf( "}\n");


	// Rimuovo gli archi dall'albero per il passo successivo
	for( i = 0; i < card_min; i++)
		GRAPHremove_edge( G, f_sol[max_set][i]);

	// Free
	for( int k = 0; k < n_sol; k++)
		free( f_sol[k]);
	free( f_sol);
}

int main( int argc, char **argv)
{
	Graph G;
	edge_t *edges;
	int card_min = INT_MAX;

	if( argc != 2) {
		fprintf( stderr, "Usare come argomento il nome del file!\n");
		exit(EXIT_FAILURE);
	}

	G = GRAPHload( argv[1]);
	GRAPHprint( stdout, G);

	// 1.
	// Trovare tutti gli isiemi di archi di cardinalita' minima
	// la cui rimozione renda il grafo un DAG

	/* La condizione del grafo per essere un DAG e' non avere
	   nessun ciclo;
	   prima rilevo tutti gli archi Back e la cardinalita' minima, poi
	   aggiungo gli isiemi alla soluzione.  */

	edges = (edge_t *)malloc( GRAPHget_edges(G)*sizeof(edge_t));
	mem_check( edges);

	// Trova la cardinalita' minima
	for( int v = 0; v < GRAPHget_vert(G); v++) {
		int len;
		if( (len = GRAPH_dfs_find_back_edges( G, edges, v)) < card_min)
			card_min = len;
	}

	if( card_min == 0)
		printf( "Il grafo e' gia un DAG!\n");
	else {
		// Il grafo viene trasformato in DAG nella funzione
		find_all_sets( G, edges, card_min);
		printf( "\nNuovo grafo:\n");
		GRAPHprint( stdout, G);
	}

	GRAPHfind_max_weight_path( G);


	free( edges);
	return 0;
}
