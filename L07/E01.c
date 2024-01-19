/* Brendon Mendicino, 271784 */

#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int u;
	int v;
} edge_t;



void ERR_out_of_mem( void);
FILE *openNewReadingFile( char *src);
void readEdgeVertex( FILE *input, int *n_vertices, int *n_edges);
edge_t *mallocEdge( int n_edges);
void readEdgesFromFile( FILE *input, edge_t *edge, int n_edges);
void generateVertexCover( edge_t *edge, int n_vertices, int n_edges);
void generateVertexCoverR(
		int pos,
		int start,
		edge_t *edge,
		int k,
		int n_vertices,
		int n_edges,
		int *vertex,
		int *sol);
int check( int *sol, edge_t *edge, int k, int n_edges);
int edgeBelong( int a, int *sol, int k);




int main( void)
{
	FILE *input;
	edge_t *edge;
	int n_edges, n_vertices;
	char *file_name = "grafo.txt";

	input = openNewReadingFile( file_name);
	readEdgeVertex( input, &n_vertices, &n_edges);

	edge = mallocEdge( n_edges);
	readEdgesFromFile( input, edge, n_edges);

	generateVertexCover( edge, n_vertices, n_edges);

	free(edge);
	fclose( input);
	return 0;
}

FILE *openNewReadingFile( char *src)
{
	FILE *input;
	if( (input = fopen( src, "r")) == NULL) {
		fprintf( stderr, "File: \"%s\" not found!\n", src);
		exit(EXIT_FAILURE);
	}
	return input;
}

edge_t *mallocEdge( int n_edges)
{
	edge_t *edge;

	edge = (edge_t *)malloc(n_edges * sizeof(edge_t));
	if( edge == NULL)
		ERR_out_of_mem();

	return edge;
}


void ERR_out_of_mem( void)
{
	fprintf( stderr, "ERROR: out of memory!\n");
	exit(EXIT_FAILURE);
}

void readEdgeVertex( FILE *input, int *n_vertices, int *n_edges)
{
	fscanf( input, "%d%d", n_vertices, n_edges);
}

void readEdgesFromFile( FILE *input, edge_t *edge, int n_edges)
{
	for( int i = 0; i < n_edges; i++) {
		fscanf( input, "%d%d", &edge[i].u, &edge[i].v);
	}
}




void generateVertexCover( edge_t *edge, int n_vertices, int n_edges)
{
	int *vertex;
	int *sol;

	// Vettore contenente gli indici dei vertici
	vertex = (int *)malloc(n_vertices * sizeof(int));
	if( vertex == NULL)
		ERR_out_of_mem();

	for( int i = 0; i < n_vertices; i++)
		vertex[i] = i;

	// Genero il PowerSet
	// Sono le combinazioni semplici da 1 a n_vertici
	for( int i = 1; i <= n_vertices; i++) {
		sol = (int *)malloc(i * sizeof(int));
		if( sol == NULL)
			ERR_out_of_mem();

		generateVertexCoverR( 0, 0, edge, i, n_vertices, n_edges, vertex, sol);
		
		free(sol);
	}

	free(vertex);
}

void generateVertexCoverR( int pos, int start, edge_t *edge, int k, int n_vertices, 
		                   int n_edges, int *vertex, int *sol)
{
	if( pos >= k) {
		if( check( sol, edge, k, n_edges) == 1) {
			printf( "W = {");
			for( int i = 0; i < k; i++)
				printf( "%d%s ", sol[i], (i == k-1)? "": ",");
			printf("}\n");
		}
		return;
	}


	/* Combinazioni semplici, non-ordinate */
	for( int i = start; i < n_vertices; i++) {
		/* Lascio il vettore dei vertici per maggiore chiarezza */
		// sol[pos] = i;
		sol[pos] = vertex[i];
		generateVertexCoverR( pos+1, i+1, edge, k, n_vertices, n_edges, vertex, sol);
	}
}

int check( int *sol, edge_t *edge, int k, int n_edges)
{
	for( int i = 0; i < n_edges; i++) {
		if( edgeBelong( edge[i].u, sol, k) == 1 || edgeBelong( edge[i].v, sol, k) == 1) {
			// Continua il ciclo
		}
		else return 0;
	}
	return 1;
}

int edgeBelong( int a, int *sol, int k)
{
	for( int i = 0; i < k; i++) {
		if( a == sol[i])
			return 1;
	}
	return 0;
}
