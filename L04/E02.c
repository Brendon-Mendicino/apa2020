/* Brendon Mendicino, 271784 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_DIM 100

int leggiVettore( char *src, int *v);
int majority( int *vettore, int l, int r);

int main( void)
{
	char line_buffer[200] = {0};
	int vettore[MAX_DIM] = {0};
	int n = 0;
	do {
		printf( "Premere invio per uscire\n");
		printf( "Inserisci il vettore:\n");
		fgets(line_buffer, 200, stdin);

		n = leggiVettore( line_buffer, vettore);

		if( n != 0)
			printf( "%d\n", majority( vettore, 0, n-1));
	} while(n != 0);

	return 0;
}



int leggiVettore( char *src, int *v)
{
	int n = 0, i = 0;
	if( src[0] == '\n') return 0;

	while( sscanf( src, "%d%n", &v[n], &i) > 0) {
		src = src+i;
		n++;
	}
	return n;
}


int majority( int *v, int l, int r)
{
	int sx, dx;
	int n_caselle;
	int min_maj;
	int m = (l+r)/2;

	if( l == r) return v[l];

	sx = majority( v, l, m);
	dx = majority( v, m+1, r);
	if( sx == dx) return sx;

	if( sx == -1 && dx == -1) return -1;


	n_caselle = ((r+1)-l);
	min_maj = (n_caselle)/2;
	/* Controllo sx */
	for( int i = l, count = 0; i <= r; i++) {
		if( v[i] == sx)
			count++;
		if( count > min_maj)
			return sx;
	}
	/* Controllo dx */
	for( int i = l, count = 0; i <= r; i++) {
		if( v[i] == dx)
			count++;
		if( count > min_maj)
			return dx;
	}
	return -1;
}
