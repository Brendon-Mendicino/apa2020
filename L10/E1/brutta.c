/* Brendon Mendicino, 271784 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
	int in;
	int fi;
} att_t;

typedef struct {
	int *sol;
	int len;
	int val;
} sol_t;


void ERR_out_of_mem( void);
att_t *initAtt( int n);
sol_t *initSol( int n);
void freeAtt( att_t *att);
att_t readAtt( FILE *input);
void attSel( int n, att_t *att);
void calcVal( int pos, att_t *att, sol_t *sol);
int check( int pos, int *sol, att_t *att);
int powerSet( int pos, int start, int n, sol_t *sol, sol_t *f_sol,  att_t *att);


int main( void)
{
	att_t *att;
	int n_att;

	printf( "Inserire il numero di attivita': ");
	scanf( "%d", &n_att);

	att = initAtt( n_att);
	printf( "Inserire il tempo iniziale ed il tempo finale:\n");
	for( int i = 0; i < n_att; i++) {
		printf( "%2d) ", i+1);
		att[i] = readAtt( stdin);
	}

	attSel( n_att, att);

	freeAtt( att);
	return 0;
}


void ERR_out_of_mem( void)
{
	fprintf( stderr, "ERROR: out of memory!\n");
	exit(EXIT_FAILURE);
}

att_t *initAtt( int n)
{
	att_t *att;

	att = (att_t *)malloc(n * sizeof(att));
	if( att == NULL)
		ERR_out_of_mem();

	return att;
}

sol_t *initSol( int n)
{
	sol_t *sol;
	sol = (sol_t *)malloc(sizeof(sol_t));
	if( sol == NULL)
		ERR_out_of_mem();

	sol->sol = (int *)malloc(n*sizeof(int));
	if( sol->sol == NULL)
		ERR_out_of_mem();
	sol->len = 0;
	sol->val = 0;

	return sol;
}

void freeAtt( att_t *att)
{
	free( att);
}

void freeSol( sol_t *sol)
{
	free( sol->sol);
	free( sol);
}

att_t readAtt( FILE *input)
{
	att_t att;

	fscanf( input, "%d%d", &att.in, &att.fi);

	return att;
}

void attSel( int n, att_t *att)
{
	sol_t *sol, *f_sol;

	sol = initSol( n);
	f_sol = initSol( n);

	powerSet( 0, 0, n, sol, f_sol, att);

	printf( "Somma durata: %d\n", f_sol->val);
	for( int i = 0; i < f_sol->len; i++)
		printf( "%2d) %d, %d\n", i+1, att[f_sol->sol[i]].in, att[f_sol->sol[i]].fi);

	freeSol( sol);
	freeSol( f_sol);
}

int powerSet( int pos, int start, int n, sol_t *sol, sol_t *f_sol, att_t *att)
{
	if( start >= n) {
		if( sol->val > f_sol->val) {
			f_sol->val = sol->val;
			f_sol->len = pos;
			for( int i = 0; i < pos; i++)
				f_sol->sol[i] = sol->sol[i];
			return 1;
		}
		return 0;
	}

	for( int i = start; i < n; i++) {
		sol->sol[pos] = i;
		if( check( pos+1, sol->sol, att)) {
			calcVal( pos+1, att, sol);
			powerSet( pos+1, i+1, n, sol, f_sol, att);
		}
	}

	if( check( pos, sol->sol, att)) {
		calcVal( pos, att, sol);
		powerSet( pos, n, n, sol, f_sol, att);
	}
	return 1;
}

int check( int pos, int *sol, att_t *att)
{
	int i = 0, j = 0;
	for( i = 0; i < pos; i++) {
		for( j = 0; j < pos; j++) {
			if( j != i)
				if( att[sol[i]].in < att[sol[j]].fi && att[sol[j]].in < att[sol[i]].fi)
					return 0;
		}
	}
	return 1;
}

void calcVal( int pos, att_t *att, sol_t *sol)
{
	sol->val = 0;
	for( int i = 0; i < pos; i++) {
		sol->val += (att[sol->sol[i]].fi - att[sol->sol[i]].in);
	}
}
