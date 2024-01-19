
#ifndef GRAPH_H
#define GRAPH_H


#include <stdio.h>

#include "symbol_t.h"


#define STR_LEN 30

/* Campo di bit per la funzione GRAPHisColplete.  */
#define MODE_MATR  1
#define MODE_LIST  2


typedef struct edge_t {
	int v;
	int w;
	int wt;
} Edge;

typedef struct graph_t *Graph;



/* Crea un arco.  */
Edge EDGEcreate( int v, int w, int wt);

/* Costruttore.  */
Graph GRAPHinit( int V);

/* Distruttore.  */
void GRAPHfree( Graph G);

/* Legge da file ed inizializza il grafo.  */
Graph GRPAHload( const char *file_name);

/* Riempe il vettore degli archi del grafo.  */
void GRAPHedges( Graph G, Edge *e);

/* Stampa il grafico.  */
void GRAPHprint( FILE *output, Graph G);

/* Ritorna se un sottografo formato da una lista di vertici e' completo.  */
/* Il campo 'mode' sceglie se operare su lista o matrice di adiacenza. */
int GRAPHisComplete( Graph G, int *list, int len, int mode);

/* Generala la lista di adiacenza.  */
void GRAPHgenerateAdjList( Graph G);

/* Inserisci un arco nel grafo.  */
void GRAPHinsertEdge( Graph G, Edge e);

/* Inserisce un arco nella lista di adiacenza.  */
void GRAPHlistInsertEdge( Graph G, Edge e);

/* Restituisci il numero preciso di vertici del grafo.  */
int GRAPHgetVert( Graph G);

/* Restituisce l'indice del 'lable' corrispondente.  */
int GRAPHgetIndexByLable( Graph G, char *lable);

/* Legge il 'lable' corrispondete all'indice.
   Se 'lable' e' NULL ritorna il puntatore all'interno della ST.  */
char *GRAPHgetLableByIndex( Graph G, int index, char *lable);

/* Trova i vertici incidenti; ritorna il numero di vertici trovati.  */
int GRAPHgetVertIncidenti( Graph G, int index, int *list);

/* Restituisce una lista con gli indici del grafico.  */
void GRAPHgetVertList( Graph G, int *list);

#endif  // GRAPH_H
