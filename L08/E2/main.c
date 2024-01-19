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
	int val_z;
	int val_r;
	int val_t;
	int val_s;
	int max_val;
	int max_len;
	int max_rip;
} pietre_t;

typedef struct {
	char *sol;
	char *final_sol;
	int z;
	int r;
	int t;
	int s;
	int sol_len;
	int sol_val;
} sol_t;


void ERR_out_of_mem( void)
{
	fprintf( stderr, "ERROR: out of memory!\n");
	exit(EXIT_FAILURE);
}

void initPietre( pietre_t *pietre);
void costruisciCatena( pietre_t *p);
void insertStones( int len, sol_t *sol);
void addStone( char stone, sol_t *sol);
void rmStone( char stone, sol_t *sol);
void calulateSolValue( sol_t *sol, pietre_t *p);
int stoneCheck( sol_t *sol, pietre_t *p);
int check( sol_t *sol, pietre_t *p);
int buildChain( int pos, int *current_sol_len, int *current_sol_val, sol_t *sol, pietre_t *p);


int main( void)
{
	pietre_t *pietre;
	pietre = (pietre_t *)calloc( 1, sizeof(pietre_t));
	if( pietre == NULL)
		ERR_out_of_mem();

	printf( "Inserire il numro di zaffiri ed il loro valore: ");
	scanf( "%d%d", &pietre->z, &pietre->val_z);
	printf( "Inserire il numro di rubini ed il loro valore: ");
	scanf( "%d%d", &pietre->r, &pietre->val_r);
	printf( "Inserire il numro di topazi ed il loro valore: ");
	scanf( "%d%d", &pietre->t, &pietre->val_t);
	printf( "Inserire il numro di smeraldi ed il loro valore: ");
	scanf( "%d%d", &pietre->s, &pietre->val_s);
	printf( "Inserire le ripetizioni massime: ");
	scanf( "%d", &pietre->max_rip);


	initPietre( pietre);

	costruisciCatena( pietre);

	free( pietre->val);
	free( pietre);
	return 0;
}

void initPietre( pietre_t *pietre)
{
	//InitPietre
	pietre->max_len += pietre->z;
	pietre->max_len += pietre->r;
	pietre->max_len += pietre->t;
	pietre->max_len += pietre->s;
	pietre->n_pietre = N_PIETRE;
	pietre->max_val = pietre->z * pietre->val_z + pietre->r * pietre->val_r +
		pietre->t * pietre->val_t + pietre->s * pietre->val_s;
	//+1 per '\0'
	pietre->val = (char *)malloc((N_PIETRE+1) * sizeof(char));
	if( pietre->val == NULL)
		ERR_out_of_mem();
	strcpy( pietre->val, "trsz");
}

void costruisciCatena( pietre_t *p)
{
	int current_sol_len = 0;
	int current_sol_val = 0;
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
	buildChain( 0, &current_sol_len, &current_sol_val, sol, p);
	//Stampa
	printf( "Lunghezza catena: %d\n", current_sol_len);
	printf( "Valore tot: %d\n", current_sol_val);
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


int buildChain( int pos, int *current_sol_len, int *current_sol_val, sol_t *sol, pietre_t *p)
{
	if( sol->sol_val >= p->max_val)
		return 1;

	if( pos >= p->max_len) {
		if( sol->sol_val >= p->max_val) {
			return 1;
		}
		return 0;
	}

	for( int i = 0; i < p->n_pietre; i++) {
		sol->sol[pos] = p->val[i];
		sol->sol_len++;
		addStone( p->val[i], sol);
		calulateSolValue( sol, p);

		if( pos >= 1  &&  check( sol, p)) {
			if( sol->sol_val > *current_sol_val) {
				*current_sol_val = sol->sol_val;
				*current_sol_len = sol->sol_len;
				strncpy( sol->final_sol, sol->sol, sol->sol_len);
			}
			if( buildChain( pos+1, current_sol_len, current_sol_val, sol, p))
				return 1;
		}
		// Stesso codice di sopra, differente solo nell'if
		// dove controllo il caso di pos=0
		else if( pos == 0 && stoneCheck( sol, p)) {
			if( sol->sol_val > *current_sol_val) {
				*current_sol_val = sol->sol_val;
				*current_sol_len = sol->sol_len;
				strncpy( sol->final_sol, sol->sol, sol->sol_len);
			}
			if( buildChain( pos+1, current_sol_len, current_sol_val, sol, p))
				return 1;
		}

		sol->sol_len--;
		rmStone( p->val[i], sol);
	}

	return 0;
}

void calulateSolValue( sol_t *sol, pietre_t *p)
{
	sol->sol_val = (sol->z*p->val_z) + (sol->r*p->val_r) + (sol->s*p->val_s) + (sol->t*p->val_t);
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

	// Il numero di zaffiri deve essere sempre minore degli smeraldi
	if( sol->z > sol->s)
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
		if( !(sol->sol[sol->sol_len-1] == 'z' || sol->sol[sol->sol_len-1] == 'r'))
			return 0;
	}
	if( sol->sol[sol->sol_len-2] == 's') {
		if( !(sol->sol[sol->sol_len-1] == 's' || sol->sol[sol->sol_len-1] == 't'))
			return 0;
	}
	if( sol->sol[sol->sol_len-2] == 'r') {
		if( !(sol->sol[sol->sol_len-1] == 's' || sol->sol[sol->sol_len-1] == 't'))
			return 0;
	}
	if( sol->sol[sol->sol_len-2] == 't') {
		if( !(sol->sol[sol->sol_len-1] == 'z' || sol->sol[sol->sol_len-1] == 'r'))
			return 0;
	}

	// Ci sono almeno 'max_rip+1' elementi nella catena
	/* Controllo che non ci siano cinque elementi ripetuti */
	if( sol->sol_len > p->max_rip) {
		int flag = 0;
		for( int i = 1; i <= p->max_rip+1; i++) {
			if( sol->sol[sol->sol_len-1] == sol->sol[sol->sol_len-i]) {
				flag++;
			}
		}
		if( flag == p->max_rip+1) return 0;
	}

	// Il numero di zaffiri deve essere sempre minore degli smeraldi
	if( sol->z > sol->s)
		return 0;

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
