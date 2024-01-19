
#include <stdio.h>
#include <stdlib.h>

#define check_pointer(A)	\
	if( (A) == NULL) {		\
		fprintf( stderr, "ERROR: out of memory!\n");	\
		exit(EXIT_FAILURE);								\
	}

typedef struct {
	int in;
	int fi;
} att_t;



att_t *initAtt( int n)
{
	att_t *att;

	att = (att_t *)malloc(n * sizeof(att));
	check_pointer( att);

	return att;
}

void freeAtt( att_t *att)
{
	free( att);
}

att_t readAtt( FILE *input)
{
	att_t att;

	fscanf( input, "%d%d", &att.in, &att.fi);

	return att;
}

int time_gap( att_t att)
{
	return att.fi- att.in;
}

int time_check( att_t a, att_t b)
{
	if( (a.in < b.fi) && (b.in < a.fi))
		return 0;
	else
		return 1;
}

int partition( att_t *att, int l, int r)
{
	int i = l-1;
	int j = r;
	att_t temp;
	att_t x = att[r]; // Pivot

#	define swap(A, B)	\
		temp = (A);		\
		(A) = (B);		\
		(B) = temp;

	for(;;) {
		while( att[++i].in < x.in);
		while( att[--j].in > x.in)
			if( j == l)
				break;
		if( i >= j)
			break;
		swap( att[i], att[j]);
	}
	swap( att[i], att[r]);

#	undef swap
	return i;
}

void quicksort( att_t *att, int l, int r)
{
	int m;
	if( l >= r)
		return;

	m = partition( att, l, r);
	quicksort( att, l, m-1);
	quicksort( att, m+1, r);
}

void constructSol( int n_att, att_t *att, int *val, int *pos)
{
	int i = n_att;
	int count = 0;

	printf( "Intervallo massimo: %d\n", val[i]);
	while( i > 0) {
		printf( "%2d) %d %d\n", count++, att[pos[i]].in, att[pos[i]].fi);
		i = pos[i];
	}
}

void attSel( int n_att, att_t *att)
{
	int *val, *pos;
	int i, j;
	
	val = (int *)calloc( n_att+1, sizeof(int));
	check_pointer(val);
	pos = (int *)malloc( (n_att+1) * sizeof(int));
	check_pointer(pos);
	
	for( int i = 0; i <= n_att; i++)
		pos[i] = -1;


	quicksort( att, 0, n_att-1);
	for( int h = 0; h < n_att; h++)
		printf( "%d %d\n", att[h].in, att[h].fi);

	/* Programmazione Dinamica.  */
	val[0] = time_gap( att[0]);
	for( i = 1; i < n_att; i++) {
		val[i] = time_gap( att[i]);
		for( j = i-1; j >= -1; j--) {
			if( time_check( att[i], att[j]) && val[j]+time_gap(att[i]) > val[i]) {
				val[i] = time_gap( att[i]) + val[j];
				pos[i] = j;
			}
		}
	}
	/* Ultimo elemento.  */
	for( j = i-1; j >= -1; j--) {
		if( val[j] > val[i]) {
			val[i] = val[j];
			pos[i] = j;
		}
	}
	
	constructSol( n_att, att, val, pos);

	free( val);
	free( pos);
}

int main( int argc, char **argv)
{
	att_t *att;
	int n_att;

	if( argc == 1) {
		printf( "Inserire il numero di attivita': ");
		scanf( "%d", &n_att);

		att = initAtt( n_att);
		printf( "Inserire il tempo iniziale ed il tempo finale:\n");
		for( int i = 0; i < n_att; i++) {
			printf( "%2d) ", i+1);
			att[i] = readAtt( stdin);
		}
	}
	else {
		FILE *in;
		in = fopen( argv[1], "r");
		if( in == NULL) {
			fprintf( stderr, "File: \"%s\" non trovato!\n", argv[1]);
			exit(EXIT_FAILURE);
		}
		fscanf( in, "%d", &n_att);

		att = initAtt( n_att);
		for( int i = 0; i <n_att; i++)
			att[i] = readAtt( in);
	}

	attSel( n_att, att);

	freeAtt( att);
	return 0;

	return 0;
}
