/* Brendon Mendicino, 271784 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N_PIETRE 4

typedef struct {
	char *val;
	int n_pietre;
	int z;
	int r;
	int t;
	int s;
	int max_len;
} pietre_t;

typedef struct {
	char *sol;
	char *final_sol;
	int z;
	int r;
	int t;
	int s;
	int sol_len;
} sol_t;


void ERR_out_of_mem( void)
{
	fprintf( stderr, "ERROR: out of memory!\n");
	exit(EXIT_FAILURE);
}

void costruisciCatena( pietre_t *p);
void insertStones( int len, sol_t *sol);
void addStone( char stone, sol_t *sol);
void rmStone( char stone, sol_t *sol);
int pruning( sol_t *sol, pietre_t *p);
int stoneCheck( sol_t *sol, pietre_t *p);
int check( sol_t *sol, pietre_t *p);
int buildChain( int pos, int *current_sol_len, sol_t *sol, pietre_t *p);


int main( void)
{
	pietre_t *pietre;
	pietre = (pietre_t *)calloc( 1, sizeof(pietre_t));
	if( pietre == NULL)
		ERR_out_of_mem();

	printf( "Inserire il numro di zaffiri: ");
	scanf( "%d", &pietre->z);
	printf( "Inserire il numro di rubini: ");
	scanf( "%d", &pietre->r);
	printf( "Inserire il numro di topazi: ");
	scanf( "%d", &pietre->t);
	printf( "Inserire il numro di smeraldi: ");
	scanf( "%d", &pietre->s);

	//InitPietre
	pietre->max_len += pietre->z;
	pietre->max_len += pietre->r;
	pietre->max_len += pietre->t;
	pietre->max_len += pietre->s;
	pietre->n_pietre = N_PIETRE;
	//+1 per '\0'
	pietre->val = (char *)malloc((N_PIETRE+1) * sizeof(char));
	if( pietre->val == NULL)
		ERR_out_of_mem();
	strcpy( pietre->val, "zrts");

	
	costruisciCatena( pietre);


	free( pietre->val);
	free( pietre);
	return 0;
}


void costruisciCatena( pietre_t *p)
{
	int current_sol_len = 0;
	sol_t *sol;
	sol = (sol_t *)calloc( 1, sizeof(sol_t));
	if( sol == NULL)
		ERR_out_of_mem();
	
	//+1 per '\0'
	sol->sol = (char *)calloc( p->max_len+1, sizeof(char));
	if( sol->sol == NULL)
		ERR_out_of_mem();
	sol->final_sol = (char *)calloc( p->max_len+1, sizeof(char));
	if( sol->sol == NULL)
		ERR_out_of_mem();

	//Alg Ricorsivo
	buildChain( 0, &current_sol_len, sol, p);
	//Stampa
	printf( "Lunghezza catena: %d\n", current_sol_len);
	for( int i = 0; i < current_sol_len; i++)
		printf( "%c%s", sol->final_sol[i], (i==current_sol_len-1)? "|": "");
	printf("\n");
	
	insertStones( current_sol_len, sol);
	printf( "Zaffiri: %d, Rubini = %d, Topazi = %d, Smeraldo %d\n",
			sol->z, sol->r, sol->t, sol->s);

	free( sol->final_sol);
	free( sol->sol);
	free( sol);
}


int buildChain( int pos, int *current_sol_len, sol_t *sol, pietre_t *p)
{

	if( pos >= p->max_len) {
		if( sol->sol_len >= p->max_len) {
			return 1;
		}
		return 0;
	}

	/* 
	 * Migliore disposizione della catena
	 * t->|z|->|r->t|->r->|s|->t
	 */
	for( int i = 0; i < p->n_pietre; i++) {
		sol->sol[pos] = p->val[i];
		sol->sol_len++;
		addStone( p->val[i], sol);

		if( pos >= 1  &&  check( sol, p)) {
			if( pruning( sol, p)){
				if( sol->sol_len > *current_sol_len) {
					*current_sol_len = sol->sol_len;
					strncpy( sol->final_sol, sol->sol, sol->sol_len);
				}
				if( buildChain( pos+1, current_sol_len, sol, p))
					return 1;
			}
		}
		// Stesso codice di sopra, differente solo nell'if
		// dove controllo il caso di pos=0
		else if( pos == 0 && stoneCheck( sol, p)) {
			if( sol->sol_len > *current_sol_len) {
				*current_sol_len = sol->sol_len;
				strncpy( sol->final_sol, sol->sol, sol->sol_len);
			}
			if( buildChain( pos+1, current_sol_len, sol, p))
				return 1;
		}

		sol->sol_len--;
		rmStone( p->val[i], sol);
	}

	return 0;
}

void addStone( char stone, sol_t *sol)
{
	switch( stone) {
		case 'z': sol->z++; break;
		case 's': sol->s++; break;
		case 'r': sol->r++; break;
		case 't': sol->t++; break;
	}
}

void rmStone( char stone, sol_t *sol) 
{
	switch( stone) {
		case 'z': sol->z--; break;
		case 's': sol->s--; break;
		case 'r': sol->r--; break;
		case 't': sol->t--; break;
	}
}

int stoneCheck( sol_t *sol, pietre_t *p)
{
	if( sol->z > p->z)
		return 0;
	if( sol->r > p->r)
		return 0;
	if( sol->t > p->t)
		return 0;
	if( sol->s > p->s)
		return 0;
	return 1;
}

int check( sol_t *sol, pietre_t *p)
{
	if( sol->z > p->z)
		return 0;
	if( sol->r > p->r)
		return 0;
	if( sol->t > p->t)
		return 0;
	if( sol->s > p->s)
		return 0;

	if( sol->sol[sol->sol_len-2] == 'z') {
		if( sol->sol[sol->sol_len-1] == 'z' || sol->sol[sol->sol_len-1] == 'r')
			return 1;
		return 0;
	}
	if( sol->sol[sol->sol_len-2] == 's') {
		if( sol->sol[sol->sol_len-1] == 's' || sol->sol[sol->sol_len-1] == 't')
			return 1;
		return 0;
	}
	if( sol->sol[sol->sol_len-2] == 'r') {
		if( sol->sol[sol->sol_len-1] == 's' || sol->sol[sol->sol_len-1] == 't')
			return 1;
		return 0;
	}
	if( sol->sol[sol->sol_len-2] == 't') {
		if( sol->sol[sol->sol_len-1] == 'z' || sol->sol[sol->sol_len-1] == 'r')
			return 1;
		return 0;
	}

	return 0;
}

int pruning( sol_t *sol, pietre_t *p)
{
	// Si devono creare le catene piu lunghe possibili di 'z' e 's'
	// finche' non finiscono tutte le pietre disponibili
	// Tutte le altre catene saranno piu corte
	if( sol->sol[sol->sol_len-2] == 'z') {
		if( sol->z < p->z && sol->sol[sol->sol_len-1] != 'z')
			return 0;
		return 1;
	}
	if( sol->sol[sol->sol_len-2] == 's') {
		if( sol->s < p->s  &&  sol->sol[sol->sol_len-1] != 's')
			return 0;
		return 1;
	}
	return 1;
}

void insertStones( int len, sol_t *sol)
{
	int i = 0;

	sol->z = 0;
	sol->s = 0;
	sol->r = 0;
	sol->t = 0;

	for( i = 0; i < len; i++) {
		switch (sol->final_sol[i]) {
			case 'z': sol->z++; break;
			case 's': sol->s++; break;
			case 'r': sol->r++; break;
			case 't': sol->t++; break;
		}
	}
}
