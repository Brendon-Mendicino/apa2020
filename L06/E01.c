/* Brendon Mendicino, 271784 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define MAX_CHAR_IN_LINE 255
#define MAX_SONGS 5

typedef struct {
	char song[MAX_SONGS][MAX_CHAR_IN_LINE];
	int n_songs;
} Friend_list_t;



void apri_file( FILE **in, char *src);
void *salva_file( FILE *in, int *n);
void crea_playlist( Friend_list_t *list, int n);
void stampa_playlist_r( char **playlist, Friend_list_t *list, int l, int r);
void stampa_playlist( char **playlist, int n);


int main( void)
{
	FILE *in;
	int n_frieds;
	Friend_list_t *list;

	apri_file( &in, "brani.txt");
	list = salva_file( in, &n_frieds);

	crea_playlist( list, n_frieds);

	free(list);
	fclose(in);
	return 0;
}


void apri_file( FILE **in, char *src)
{
	if( (*in = fopen( src, "r")) == NULL) {
		fprintf( stderr, "File: \"%s\" non trovato!\n", src);
		exit(1);
	}
}


void *salva_file( FILE *in, int *n)
{
	Friend_list_t *list;
	int i = 0, j = 0;

	fscanf( in, "%d", n);
	list = (Friend_list_t *)malloc(*n *sizeof(Friend_list_t));
	if( list == NULL)
		exit(1);

	for( i = 0; i < *n; i++) {
		fscanf( in, "%d", &list[i].n_songs);
		for( j = 0; j < list[i].n_songs; j++)
			fscanf( in, "%s", list[i].song[j] );
	}

	return (void *)list;
}


void crea_playlist( Friend_list_t *list, int n)
{
	int i = n-1;
	char **playlist = (char **)malloc(n*sizeof(char *));
	stampa_playlist_r( playlist, list, 0, i);
	free(playlist);
}


void stampa_playlist_r( char **playlist, Friend_list_t *list, int l, int r)
{
	int i = 0;
	if( l > r) {
		stampa_playlist( playlist, r);
		return;
	}

	for( i = 0; i < list[l].n_songs; i++) {
		playlist[l] = list[l].song[i];
		stampa_playlist_r( playlist, list, l+1, r);
	}
}

void stampa_playlist( char **playlist, int n)
{
	for(int i = 0; i <= n; i++)
		printf( "%s\n", playlist[i]);
	printf( "\n");
}
