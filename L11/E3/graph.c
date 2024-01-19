#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"


typedef struct node *link_t;

struct node {
	int v;
	int wt;
	link_t next;
};

struct graph_t {
	int V;
	int E;
	int **madj;
	link_t *ladj;
	link_t z; /* Sentinella.  */
	ST tab;
};




static void memCheck( void *p)
{
	if( p == NULL) {
		fprintf( stderr, "ERROR: out of memory!\n");
		exit(EXIT_FAILURE);
	}
}

static int **MatrInit( int rows, int cols, int val)
{
	int **m;
	m = (int **)malloc( rows * sizeof(int *));
	memCheck( m);
	for( int i = 0; i < rows; i++) {
		m[i] = (int *)malloc( cols * sizeof(int));
		memCheck( m[i]);
		// Inizializzo al valore passato
		for( int j = 0; j < cols; j++)
			m[i][j] = val;
	}
	return m;
}

static link_t NewNode( int v, int wt, link_t next)
{
	link_t x;
	x = (link_t)malloc( sizeof(*x));
	memCheck( x);
	x->next = next;
	x->wt   = wt;
	x->v    = v;
	return x;
}

static void freeListR( link_t x, link_t z)
{
	if( x == z)
		return;
	freeListR( x->next, z);
	free( x);
}

static int convertNet( char *net)
{
	static char *net_alias[] = {
		"Net1",
		"Net2",
		"Net3"
	};

	for( int i = 0; i < 3; i++) {
		if( strcmp( net_alias[i], net) == 0)
			return i+1;
	}
	return 0;
}



/* Crea un arco.  */
Edge EDGEcreate( int v, int w, int wt)
{
	Edge e;
	e.v = v;
	e.w = w;
	e.wt = wt;
	return e;
}

/* Costruttore.  */
Graph GRAPHinit( int V)
{
	Graph G;
	G = (Graph)malloc( sizeof(*G));
	memCheck( G);
	G->V = V;
	G->E = 0;
	G->madj = MatrInit( V, V, 0);
	G->tab = STinit( V);
	
	G->z = NewNode( -1, -1, NULL);
	G->ladj = (link_t *)malloc( V * sizeof(link_t));
	memCheck( G->ladj);
	for( int i = 0; i < V; i++)
		G->ladj[i] = G->z;
	return G;
}

/* Distruttore.  */
void GRAPHfree( Graph G)
{
	for( int i = 0; i < G->V; i++)
		free( G->madj[i]);
	free( G->madj);

	for( int i = 0; i < G->V; i++)
		freeListR( G->ladj[i], G->z);
	free( G->ladj);

	STfree( G->tab);
	free( G);
}



/* Legge da file ed inizializza il grafo.  */
Graph GRPAHload( const char *file_name)
{
	FILE *input;
	int buff;
	int V = 0;
	Graph G;

	int id1, id2;
	int wt;
	char label1[STR_LEN];
	char label2[STR_LEN];
	char net1[STR_LEN];
	char net2[STR_LEN]; 

	input = fopen( file_name, "r");
	if( input == NULL) {
		fprintf( stderr, "File: \"%s\" not found!\n", file_name);
		exit(EXIT_FAILURE);
	}

	// Conto le righe del file
	while( (buff = fgetc( input)) != EOF)
		if( buff == '\n')
			V++;

	rewind( input);

	G = GRAPHinit( 2*V);

	while( fscanf( input, "%s%s%s%s%d", label1, net1,
				label2, net2, &wt) == 5) {
		id1 = STgetValue( G->tab, label1);
		id2 = STgetValue( G->tab, label2);
		if( id1 == -1)
			id1 = STinsert( G->tab, label1, convertNet( net1));
		if( id2 == -1)
			id2 = STinsert( G->tab, label2, convertNet( net2));

		GRAPHinsertEdge( G, EDGEcreate( id1, id2, wt));
	}
	
	fclose( input);
	return G;
}

/* Riempe il vettore degli archi del grafo.  */
void GRAPHedges( Graph G, Edge *e)
{
	int E = 0;
	int i, j;
	// Non orientato
	for( i = 0; i < G->V; i++) {
		for( j = i+1; j < G->V; j++) {
			if( G->madj[i][j] != 0)
				e[E++] = EDGEcreate( i, j, G->madj[i][j]);
		}
	}
}

/* Stampa il grafico.  */
void GRAPHprint( FILE *output, Graph G)
{
	Edge *e;

	e = (Edge *)malloc( G->E * sizeof(Edge));
	memCheck( e);

	GRAPHedges( G, e);

	for( int i = 0; i < G->E; i++)
		fprintf( output, "%s %s %d\n", STsearchByValue( G->tab, e[i].v),
				STsearchByValue( G->tab, e[i].w), e[i].wt);
	free( e);
}

/* Trova i vertici incidenti; ritorna il numero di vertici trovati.  */
int GRAPHgetVertIncidenti( Graph G, int index, int *list)
{
	int n_incidenti = 0;

	// Grafo non-orientato
	for( int i = 0; i < G->V; i++) {
		if( G->madj[i][index] != 0) {
			list[n_incidenti] = i;
			n_incidenti++;
		}
	}
	return n_incidenti;
}

/* Restituisce se un sottografo formato da una lista di vertici e' un
   sottografo completo.  */
int GRAPHisComplete( Graph G, int *list, int len, int mode)
{
	/* Il vettore 'list' contiene i vari vertici, e 'len' e'
	   la lunghezza del vettore.  */

	/* Applico la definizione di sotto grafo completo:
	   Per ogni vertice i,j  con i!=j
	   Esiste (i,j) che appartine a E.   */
	// Grafo non orientato
	if( mode & MODE_MATR) {
		// Se un un arco non risetta il criterio ritorno 0.
		for( int i = 0; i < len; i++) {
			for( int j = 0; j < len; j++) {
				if( j == i)
					continue;
				if( G->madj[list[i]][list[j]] == 0)
					return 0;
			}
		}
	}
	else if( mode & MODE_LIST) {
		link_t temp;
		int check = 0;
		if( G->ladj == NULL)
			return 0;

		for( int i = 0; i < len; i++) {
			for( int j = 0; j < len; j++) {
				if( j == i)
					continue;
				check = 0;
				temp = G->ladj[list[i]];
				// Controlle se l'arco (i,j) esiste
				// percorrendo la lista
				while( temp != G->z) {
					if( temp->v == list[j]) {
						check = 1;
						break;
					}
					temp = temp->next;
				}
				if( check == 0)
					return 0;
			}
		}
	}
	return 1;
}

/* Generala la lista di adiacenza.  */
void GRAPHgenerateAdjList( Graph G)
{
	int v, w;
	// Grafo non-orientato
	for( v = 0; v < G->V; v++) {
		for( w = v+1; w < G->V; w++) {
			if( G->madj[v][w] != 0)
				GRAPHlistInsertEdge( G, EDGEcreate( v, w, G->madj[v][w]));
		}
	}
}

/* Inserisci un arco nel grafo.  */
void GRAPHinsertEdge( Graph G, Edge e)
{
	if( G->madj[e.v][e.w] == 0)
		G->E++;
	// Non orientato
	G->madj[e.v][e.w] = e.wt;
	G->madj[e.w][e.v] = e.wt;
}

/* Inserisce un arco nella lista di adiacenza.  */
void GRAPHlistInsertEdge( Graph G, Edge e)
{
	// Grafo non-orientato
	G->ladj[e.v] = NewNode( e.w, e.wt, G->ladj[e.v]);
	G->ladj[e.w] = NewNode( e.v, e.wt, G->ladj[e.w]);
	G->E++;
}


/* Restituisci il numero preciso di vertici del grafo.  */
int GRAPHgetVert( Graph G)
{
	return STcount( G->tab);
}

/* Restituisce l'indice del 'lable' corrispondente.  */
int GRAPHgetIndexByLable( Graph G, char *lable)
{
	return STgetValue( G->tab, lable);
}

/* Legge il 'lable' corrispondete all'indice.
   Se 'lable' e' NULL ritorna il puntatore all'interno della ST.  */
char *GRAPHgetLableByIndex( Graph G, int index, char *lable)
{
	char *buff;
	buff = STsearchByValue( G->tab, index);
	if( lable != NULL) {
		strcpy( lable, buff);
		return lable;
	}
	return buff;
}


/* Restituisce una lista con gli indici del grafico.  */
void GRAPHgetVertList( Graph G, int *list)
{
	int n_items;

	n_items = STcount( G->tab);
	for( int i = 0; i < n_items; i++)
		list[i] = STselect( G->tab, i);
}
