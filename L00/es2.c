#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 21

int conta( char S[MAX], int n);

int main( void)
{
	FILE *input;
	char stringa[MAX] = {0}, nome_file[20] = {0};
	int len_stringhe = 0, num_righe = 0, num_parole = 0;

	printf( "Inserire il nome del file: ");
	scanf( "%s", nome_file);

	if( (input = fopen( nome_file, "r") ) == NULL ) {
		fprintf( stderr, "File non trovato!\n");
		exit(1);
	}

	printf( "Inserire la lunghezza delle sottostringhe: ");
	scanf( "%d", &len_stringhe);

	fscanf( input, "%d\n", &num_righe);
	
	while( fgets( stringa, 21, input) != NULL) {
		for( int i = 0; i < MAX; i++) {
			if( stringa[i] == '\n') {
				stringa[i] = '\0';
			}
		}
		num_parole = conta( stringa, len_stringhe);
		fprintf( stdout, "Ci sono %d sottostringhe in %s!\n", num_parole, stringa);
	}
	
	fclose(input);
	return 0;
}

int conta( char S[MAX], int n)
{
	int vocale = 0, num_stringhe = 0;
	int stop = (int)strlen(S) - n + 1;
	for( int i = 0; i < stop; i++) {
		int j = 0; 
		for( j = 0; j < n; j++) {
			switch (S[i + j]) {
				case 'a':
				case 'e':
				case 'i':
				case 'o':
				case 'u': vocale++;
			}
		}
		if( vocale >= 2) { 
			num_stringhe++;
		}
		
		vocale = 0;
	}
	return num_stringhe;	
}
