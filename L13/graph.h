
#ifndef GRAPH_H
#define GRAPH_H


#include <stdio.h>

#define MAX_VERTICI  30



typedef struct {
	int v;
	int w;
	int wt;
} edge_t;

typedef struct graph *Graph;


Graph GRAPHnull( void);

Graph GRAPHinit( int max);

void GRAPHfree( Graph G);

int GRAPHget_vert( Graph G);

int GRAPHget_edges( Graph G);

void GRAPHinsertEdge( Graph G, edge_t e);

void GRAPHremove_edge( Graph G, edge_t e);

Graph GRAPHload( const char *file_name);

void GRAPHprint( FILE *output, Graph G);

void GRAPHcopy( Graph dest, Graph src);

/* Effettua una DFS sull'albero e trova tutti gli archi Back; retituisce
   la lunghezza del vettore.  */
int GRAPH_dfs_find_back_edges( Graph G, edge_t *edges, int initial_v);

void GRAPHfind_max_weight_path( Graph G);

#endif
