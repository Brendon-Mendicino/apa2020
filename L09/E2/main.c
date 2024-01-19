/* Brendon Mendicino, 271784 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 200
#define N_ROTATION 2

typedef struct {
	char c[N_ROTATION];
	int val[N_ROTATION];
} tile_t;

typedef struct {
	int tile;
	int rot;
} square_t;

typedef struct {
	square_t **s;
	int rows;
	int cols;
} board_t;

typedef struct {
	board_t *board;
	int val;
} sol_t;



void ERR_out_of_mem(void);

FILE *openReadingFile( const char *file_name);
tile_t * initTiles( const int n);
board_t *initBoard( const int rows, const int cols);
void *initVec( const int n, size_t size);
int **initMark( int rows, int cols);
sol_t *initSol( board_t *board);

void freeTiles( tile_t *tile);
void freeBoard( board_t *board);
void freeMark( int rows, int **mark);
void freeSol( sol_t *sol);

void printBoard( FILE *output, board_t *board);

tile_t *readFileAndInitTile( char *file_name, int *n_tiles);
board_t *readFileAndInitBoard( char *file_name);

void boardCopy( board_t *a, board_t *b);
int rowCheck( board_t *board, int row, tile_t *tile);
int colCheck( board_t *board, int col, tile_t *tile);
int getSolVal( sol_t *sol, tile_t *tile);

void findMaxPoints( int n_tiles, tile_t *tile, board_t *board);
int permutazioni( int pos, int n_tiles, board_t *board, tile_t *tile, sol_t *sol, sol_t *f_sol, int **mark);

void setMark( board_t *board, int **mark);



int main( void)
{
	board_t *board;
	tile_t *tile;
	int n_tiles = 0;

	tile = readFileAndInitTile( "tiles.txt", &n_tiles);
	board = readFileAndInitBoard( "board.txt");

	printBoard( stdout, board);
	for( int i = 0; i < n_tiles; i++)
		printf( "%2d) %c %d %c %d\n", i, tile[i].c[0], tile[i].val[0], tile[i].c[1], tile[i].val[1]);

	findMaxPoints( n_tiles, tile, board);

	freeTiles( tile);
	freeBoard( board);
	return 0;
}



void ERR_out_of_mem(void)
{
	fprintf( stderr, "ERROR: out of memoruy!\n");
	exit(EXIT_FAILURE);
}

FILE *openReadingFile( const char *file_name)
{
	FILE *input;
	if( (input = fopen( file_name, "r")) == NULL) {
		fprintf( stderr, "File: \"%s\" not found!\n", file_name);
		exit(EXIT_FAILURE);
	}
	return input;
}

void *initVec( const int n, size_t size)
{
	void *vec;

	vec = (void *)malloc(n * size);
	if( vec == NULL)
		ERR_out_of_mem();
	return vec;
}

tile_t *initTiles( const int n)
{
	tile_t *tile;

	tile = (tile_t *)malloc(n*sizeof(tile_t));
	if( tile == NULL)
		ERR_out_of_mem();

	return tile;
}

board_t *initBoard( const int rows, const int cols)
{
	board_t *board;

	board = (board_t *)malloc(sizeof(board_t));
	if( board == NULL)
		ERR_out_of_mem();

	board->s = (square_t **)malloc(rows * sizeof(square_t *));
	if( board->s == NULL)
		ERR_out_of_mem();

	for( int i = 0; i < rows; i++) {
		board->s[i] = (square_t *)malloc(cols * sizeof(square_t));
		if( board->s[i] == NULL)
			ERR_out_of_mem();
	}

	board->rows = rows;
	board->cols = cols;
	return board;
}

int **initMark( int rows, int cols)
{
	int **mark;

	mark = (int **)initVec( rows, sizeof(int *));
	for( int i = 0; i < rows; i++) {
		mark[i] = (int *)initVec( cols, sizeof(int));
	}
	return mark;
}

 void freeTiles( tile_t *tile)
{
	free( tile);
}

void freeBoard( board_t *board)
{
	for( int i = 0; i < board->rows; i++)
		free( board->s[i]);
	free( board->s);
	free( board);
}

void freeMark( int rows, int **mark)
{
	for( int i = 0; i < rows; i++)
		free( mark[i]);
	free( mark);
}

tile_t *readFileAndInitTile( char *file_name, int *n_tiles)
{
	int i = 0;
	tile_t *tile;
	FILE *input;
	char line_buffer[MAX_LINE];

	input = openReadingFile( file_name);
	fgets( line_buffer, MAX_LINE, input);
	sscanf( line_buffer, "%d", n_tiles);

	tile = initTiles( *n_tiles);

	for( i = 0; i < *n_tiles; i++) {
		fgets( line_buffer, MAX_LINE, input);
		sscanf( line_buffer, "%[A-Z]%d%*[ ]%[A-Z]%d", &tile[i].c[0], &tile[i].val[0], &tile[i].c[1], &tile[i].val[1]);
	}

	fclose( input);
	return tile;
}

board_t *readFileAndInitBoard( char *file_name)
{
	board_t *board;
	FILE *input;
	char line_buffer[MAX_LINE];
	char *temp;
	int increment = 0;
	int rows = 0;
	int cols = 0;

	input = openReadingFile( file_name);
	fgets( line_buffer, MAX_LINE, input);
	sscanf( line_buffer, "%d%d", &rows, &cols);

	board = initBoard( rows, cols);

	int i, j;
	for( i = 0; i < board->rows; i++) {
		fgets( line_buffer, MAX_LINE, input);
		temp = line_buffer;
		for( j = 0; j < board->cols; j++) {
			sscanf( temp, "%d/%d%n", &board->s[i][j].tile, &board->s[i][j].rot, &increment);
			temp = temp + increment;
		}
	}

	fclose(input);
	return board;
}

void printBoard( FILE *output, board_t *board)
{
	int i, j;
	for( i = 0; i < board->rows; i++) {
		for( j = 0; j < board->cols; j++)
			fprintf( output, "%d/%d%s", board->s[i][j].tile, board->s[i][j].rot, (j == board->cols-1)? "": " ");
		fprintf( output, "\n");
	}
}



sol_t *initSol( board_t *board)
{
	sol_t *sol;
	
	sol = (sol_t *)malloc(sizeof(sol_t));
	sol->board = initBoard( board->rows, board->cols);

	sol->board->rows = board->rows;
	sol->board->cols = board->cols;
	for( int i = 0; i < board->rows; i++) {
		for( int j = 0; j < board->cols; j++) {
			sol->board->s[i][j] = board->s[i][j];
		}
	}
	sol->val = 0;

	return sol;
}

void freeSol( sol_t *sol)
{
	freeBoard( sol->board);
	free( sol);
}

void findMaxPoints( int n_tiles, tile_t *tile, board_t *board)
{
	sol_t *sol;
	sol_t *f_sol;
	int **mark;

	sol = initSol( board);
	f_sol = initSol( board);
	mark = initMark( board->rows, board->cols);
	setMark( board, mark);
	
	// Modello: Permutazioni semplici
	permutazioni( 0, n_tiles, board, tile, sol, f_sol, mark);

	// Stampa
	printf( "Valore finale: %d\n", f_sol->val);
	printBoard( stdout, f_sol->board);

	freeMark( board->rows, mark);
	freeSol( sol);
	freeSol( f_sol);
}

void setMark( board_t *board, int **mark)
{
	int i, j;
	for( i = 0; i < board->rows; i++) {
		for( j = 0; j < board->cols; j++) {
			if( board->s[i][j].tile == -1)
				mark[i][j] = 0;
			else
				mark[i][j] = 1;
		}
	}
}



int rowCheck( board_t *board, int row, tile_t *tile)
{
	for( int j = 0; j < board->cols-1; j++) {
		#define _tile(A, B)  board->s[(A)][(B)].tile
		#define _rot(A, B)   board->s[(A)][(B)].rot

		if( tile[_tile(row, j)].c[_rot(row, j)] !=
			tile[_tile(row, j+1)].c[_rot(row, j+1)]) return 0;

		#undef _tile
		#undef _rot
	}
	return 1;
}

int colCheck( board_t *board, int col, tile_t *tile)
{
	for( int i = 0; i < board->rows-1; i++) {
		#define _tile(A, B)  board->s[(A)][(B)].tile
		#define _rot(A, B)   board->s[(A)][(B)].rot

		if( tile[_tile(i, col)].c[(_rot(i, col)+1) %2] !=
		    tile[_tile(i+1, col)].c[(_rot(i+1, col)+1) %2]) return 0;

		#undef _tile
		#undef _rot
	}
	return 1;
}

int getSolVal( sol_t *sol, tile_t *tile)
{
	int val = 0;
	int i, j;

	#define _tile(A, B)  sol->board->s[(A)][(B)].tile
	#define _rot(A, B)   sol->board->s[(A)][(B)].rot
	// Per righe
	for( i = 0; i < sol->board->rows; i++) {
		if( rowCheck( sol->board, i, tile)) {
			for( j = 0; j < sol->board->cols; j++) {
				val += tile[_tile(i, j)].val[_rot(i, j)];
			}
		}
	}
	// Per colonne
	for( j = 0; j < sol->board->cols; j++) {
		if( colCheck( sol->board, j, tile)) {
			for( i = 0; i < sol->board->rows; i++) {
				val += tile[_tile(i, j)].val[(_rot(i, j)+1) %2];
			}
		}
	}
	#undef _tile
	#undef _rot
	return val;
}

int permutazioni( int pos, int n_tiles, board_t *board, tile_t *tile, sol_t *sol, sol_t *f_sol, int **mark)
{
	// i = riga, j = colonna
	int i, j;
	if( pos >= board->cols * board->rows) {
		sol->val = getSolVal( sol, tile);
		if( sol->val > f_sol->val) {
			f_sol->val = sol->val;
			boardCopy( f_sol->board, sol->board);
		}
		return 0;
	}

	i = pos / board->cols;
	j = pos % board->cols;
	if( board->s[i][j].tile != -1)
		permutazioni( pos+1, n_tiles, board, tile, sol, f_sol, mark);
	else {
		for( int x = 0; x < n_tiles; x++) {
			if( mark[i][j] == 0) {
				mark[i][j] = 1;
				sol->board->s[i][j].tile = x;

				sol->board->s[i][j].rot = 0;
				permutazioni( pos+1, n_tiles, board, tile, sol, f_sol, mark);

				sol->board->s[i][j].rot = 1;
				permutazioni( pos+1, n_tiles, board, tile, sol, f_sol, mark);

				mark[i][j] = 0;
			}
		}
	}

	return 0;
}

void boardCopy( board_t *a, board_t *b)
{
	int i, j;
	for( i = 0; i < a->rows; i++) {
		for( j = 0; j < a->cols; j++) {
			a->s[i][j] = b->s[i][j];
		}
	}
}
