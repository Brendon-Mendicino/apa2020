
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "graph.h"
#include "symbol_t.h"




typedef struct node *link_t;

struct node {
	int v;
	int wt;
	link_t next;
};

struct graph {
	int V;
	int E;
	link_t *ladj;
	link_t z;  /* Sentinella */
	ST tab;
};




static void mem_check( void *p)
{
	if( p == NULL) {
		fprintf( stderr, "ERROR: out of memory!\n");
		exit(EXIT_FAILURE);
	}
}

static link_t new_node( link_t next, int v, int wt)
{
	link_t new;
	new = (link_t)malloc(sizeof(struct node));
	mem_check( new);

	new->v    = v;
	new->wt   = wt;
	new->next = next;
	return new;
}

static edge_t edge_create( int v, int w, int wt)
{
	edge_t e;
	e.v  = v;
	e.w  = w;
	e.wt = wt;
	return e;
}






Graph GRAPHnull( void)
{
	return NULL;
}

Graph GRAPHinit( int max)
{
	Graph G;
	G = (Graph)malloc( sizeof(struct graph));
	mem_check( G);

	G->V = max;
	G->E = 0;

	G->z = new_node( NULL, 0, 0);
	G->ladj = (link_t *)malloc( G->V * sizeof(link_t));
	mem_check( G->ladj);
	for( int i = 0; i < G->V; i++) {
		G->ladj[i] = G->z;
	}
	G->tab = STinit( max);

	return G;
}

void GRAPHfree( Graph G)
{
	link_t x, temp;
	STfree( G->tab);
	// Libero la lista di adiacenza
	for( int i = 0; i < G->V; i++) {
		temp = G->ladj[i];
		while( temp != G->z) {
			x = temp;
			temp = temp->next;
			free( x);
		}
	}
	free( G->z);
	free( G);
}

int GRAPHget_vert( Graph G)
{
	return G->V;
}

int GRAPHget_edges( Graph G)
{
	return G->E;
}

void GRAPHinsert_edge( Graph G, edge_t e)
{
	G->ladj[e.v] = new_node( G->ladj[e.v], e.w, e.wt);
	G->E++;
}

void GRAPHremove_edge( Graph G, edge_t e)
{
	link_t x, prev;
	int found = 0;

	// Caso in testa
	if( G->ladj[e.v]->v == e.w) {
		found = 1;
		x = G->ladj[e.v];
		G->ladj[e.v] = G->ladj[e.v]->next;
		free( x);
	}
	// Caso normale
	else if( G->ladj[e.v] != G->z) {
		prev = G->ladj[e.v];
		x = prev->next;
		while( x != G->z) {
			if( x->v == e.w) {
				found = 1;
				prev->next = x->next;
				free( x);
				break;
			}
			prev = x;
			x = x->next;
		}
	}
	
	if( found)
		G->E--;
}

Graph GRAPHload( const char *file_name)
{
	Graph G;
	int n_v;
	FILE *input;
	char str_1[40];
	char str_2[40];
	int id_1;
	int id_2;
	int weight;
	

	input = fopen( file_name, "r");
	if( input == NULL) {
		fprintf( stderr, "File: \"%s\" not found!\n", file_name);
		exit(EXIT_FAILURE);
	}

	fscanf( input, "%d", &n_v);
	G = GRAPHinit( n_v);

	for( int i = 0; i < G->V; i++) {
		fscanf( input, "%s", str_1);
		STinsert( G->tab, str_1);
	}
	while( fscanf( input, "%s%s%d", str_1, str_2, &weight) == 3) {
		id_1 = STsearch( G->tab, str_1);
		id_2 = STsearch( G->tab, str_2);
		GRAPHinsert_edge( G, edge_create( id_1, id_2, weight));
	}

	fclose( input);
	return G;
}


void GRAPHprint( FILE *output, Graph G)
{
	link_t temp;
	int count = 0;
	for( int i = 0; i < G->V; i++) {
		temp = G->ladj[i];
		while( temp != G->z) {
			fprintf( output, "%3d)%s %s: %d\n", ++count, STsearchByValue( G->tab, i),
					 STsearchByValue( G->tab, temp->v), temp->wt);
			temp = temp->next;
		}
	}
}



void GRAPHcopy( Graph dest, Graph src)
{
	link_t src_temp;
	dest->V = src->V;
	dest->E = src->E;
	// Non c'e' bisogno della STcopy

	for( int i = 0; i < src->V; i++) {
		src_temp  = src->ladj[i];
		while( src_temp != src->z) {
			dest->ladj[i] = new_node( dest->ladj[i], src_temp->v, src_temp->wt);
			src_temp = src_temp->next;
		}
	}
}


static void dfs_find_back( edge_t *edges, int *arr_len, Graph G, int v,
		                   int *fathers, int *pre, int *post, int *time)
{
	link_t x;

	pre[v] = (*time)++;
	for( x = G->ladj[v]; x != G->z; x = x->next) {
		if( pre[x->v] == -1) {
			fathers[x->v] = v;
			dfs_find_back( edges, arr_len, G, x->v, fathers, pre, post, time);
		}
		else if( post[x->v] == -1) {
			edges[(*arr_len)++] = edge_create( v, x->v, x->wt);
		}
	}
	post[v] = (*time)++;
}


/* Effettua una DFS sull'albero e trova tutti gli archi Back; retituisce
   la lunghezza del vettore.  */
int GRAPH_dfs_find_back_edges( Graph G, edge_t *edges, int initial_v)
{
	int *fathers, *pre, *post;
	int time = 0;
	int arr_len = 0;

	/* Inizializzazione.  */
	fathers = (int *)malloc( G->V * sizeof(int));
	mem_check( fathers);
	pre = (int *)malloc( G->V * sizeof(int));
	mem_check( pre);
	post = (int *)malloc( G->V * sizeof(int));
	mem_check( post);

	for( int i = 0; i < G->V; i++) {
		fathers[i] = i;
		post[i] = -1;
		pre[i] = -1;
	}

	// Comincio dal primo vertice mandato dal client
	dfs_find_back( edges, &arr_len, G, initial_v, fathers, pre, post, &time);
	for( int i = 0; i < G->V; i++) {
		if( pre[i] == -1)
			dfs_find_back( edges, &arr_len, G, i, fathers, pre, post, &time);
	}

	free( fathers);
	free( pre);
	free( post);
	return arr_len;
}

static void GRAPHdfs_ord_top( Graph G, int *ord, int *time, int *pre, int v)
{
	link_t a;
	pre[v] = 0;
	for( a = G->ladj[v]; a != G->z; a = a->next) {
		if( pre[a->v] == -1)
			GRAPHdfs_ord_top( G, ord, time, pre, a->v);
	}
	// Ordinamento topologico
	// 'time' parte da G->V
	ord[--(*time)] = v;
}


void GRAPHfind_ordinamento_topologico( Graph G, int *ord)
{
	int *pre;
	int time = G->V;
pre = (int *)malloc( G->V*sizeof(int));
	mem_check( pre);

	for( int v = 0; v < G->V; v++) {
		pre[v] = -1;
	}

	for( int v = 0; v < G->V; v++) {
		if( pre[v] == -1)
			GRAPHdfs_ord_top( G, ord, &time, pre, v);
	}

	free(pre);
}


static int find_source( Graph G, int *source_list)
{
	link_t a;
	int n_sources = G->V;

	for( int v = 0; v < G->V; v++) {
		for( a = G->ladj[v]; a != G->z; a = a->next) {
			if( source_list[a->v] != 1) {
				source_list[a->v] = 1;
				n_sources--;
			}
		}
	}
	return n_sources;
}


static void print_from_father( Graph G, int *father, int *d, int source)
{
	for( int i = 0; i < G->V; i++) {
		if( i!=source && father[i] == source) {
			printf( "Distanza da \"%s\": %d\n", STsearchByValue( G->tab, i), d[i]);
			print_from_father( G, father, d, i);
		}
	}
}

void GRAPHfind_max_weight_path( Graph G)
{
	// 3.
	// Trovare tutti i cammini massimimi dei nodi sorgente

	link_t a;
	int *father, *d, *ord;
	int source;
	int *source_list;
	int v;


	// Inizializzazione
	father = (int *)malloc( G->V*sizeof(int));
	mem_check(father);
	d = (int *)malloc( G->V*sizeof(int));
	mem_check( d);
	ord = (int *)malloc( G->V*sizeof(int));
	mem_check( ord);
	source_list = (int *)calloc( G->V, sizeof(int));
	mem_check( source_list);


	// Cerco l'ordine topologico
	GRAPHfind_ordinamento_topologico( G, ord);

	// Costrusico il vettore delle sorgenti:
	// se 'source_list[v] = 0', allora il vertice 'v' e' una sorgente
	find_source( G, source_list);


	// Prima di cercare i cammini massimi trovo il nodo sorgente
	for( source = 0; source < G->V; source++) {
		int start;
		// Trovo la sorgente
		if( source_list[source] != 0)
			continue;

		// Resetto i vettori
		for( v = 0; v < G->V; v++) {
			father[v] = v;
			d[v] = INT_MIN;
		}

		// Trovo da quale vertice partire
		for( int i = 0; i < G->V; i++) {
			if( ord[i] == source) {
				start = i;
				break;
			}
		}

		// Ricerca
		d[source] = 0;
		for( v = start; v < G->V; v++) {
			if( d[ord[v]] != INT_MIN) {
				for( a = G->ladj[ord[v]]; a != G->z; a = a->next) {
					if( d[ord[v]] + a->wt > d[a->v]) {
						d[a->v] = d[ord[v]] + a->wt;
						father[a->v] = ord[v];
					}
				}
			}
		}

		// Stampa
		printf( "\nVertice sorgente: %s\n", STsearchByValue( G->tab, source));
		print_from_father( G, father, d, source);
	}
}
