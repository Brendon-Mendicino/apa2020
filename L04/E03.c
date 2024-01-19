/* Brendon Mendicno, 271784 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LEN 200

char *cercaRegexp( char *src, char *regexp);
int checkForBSlash( char regexp, char src);
int checkInsideBrackets( char *regexp, int *reg_count, char src);
int calcolaLenRegexp( char *regexp);


int main( void)
{
	char str[MAX_LINE_LEN] = {0};
	char regexp[MAX_LINE_LEN] = {0};
	int loop = 1;
	
	while(loop) {
		printf( "Scrivi una stringa: ");
		fgets( str, MAX_LINE_LEN, stdin);
		printf( "Inserisci l'espressione regolare che vuoi cercare: ");
		fgets( regexp, MAX_LINE_LEN, stdin);
		/* rimuovo il \n */
		str[strlen(str)-1] = '\0';
		regexp[strlen(regexp)-1] = '\0';

		printf( "%s\n\n", cercaRegexp( str, regexp));
	}

}


char *cercaRegexp( char *src, char *regexp)
{
	int lunghezza_regexp = calcolaLenRegexp( regexp);
	int i = 0, j = 0;

	int flag = 1;
	for( int h = 0; src[h+lunghezza_regexp-1] != '\0'; h++) {
		char *src_buff = &src[h];

		flag = 1;
		/* i = regex_counter
		 * j = src_counter */
		for( i = 0, j = 0; regexp[i] != '\0' && flag == 1; i++, j++) {
			if( isalnum( regexp[i])) {
				if( regexp[i] != src_buff[j])
					flag = 0;
			}
			else if( regexp[i] == ' ') {
				if( src_buff[j] != ' ')
					flag = 0;
			}
			else if( regexp[i] == '.') {
				// all printable chars are ok
				if( !isprint( src_buff[j]))
					flag = 0;
			}
			else if( regexp[i] == '\\') {
				i++;
				flag = checkForBSlash( regexp[i], src_buff[j]);
			}
			else if( regexp[i] == '[') {
				i++;
				flag = checkInsideBrackets( &regexp[i], &i, src_buff[j]);
			}
		}

		if( j == lunghezza_regexp && flag == 1) {
			src_buff[lunghezza_regexp] = '\0';
			return src_buff;
		}
	}

	return NULL;
}


int checkForBSlash( char regexp, char src)
{
	int flag = 1;
	if( regexp == 'a') {
		if( !islower( src))
			flag = 0;
	}
	else if( regexp == 'A') {
		if( !isupper( src))
			flag = 0;
	}

	return flag;
}


int checkInsideBrackets( char *regexp, int *reg_count, char src)
{
	int flag = 1;
	int i = 0;

	if( regexp[i] != '^') {
		flag = 0;
		while( regexp[i] != ']') {
			if( regexp[i] == src)
				flag = 1;
			i++;
		}
	}
	else {
		flag = 1;
		while( regexp[i] != ']') {
			if( regexp[i] == src)
				flag = 0;
			i++;
		}
	}

	*reg_count += i;
	return flag;
}


int calcolaLenRegexp( char *regexp)
{
	int i = 0, len = 0;
	for( i = 0; regexp[i] != '\0'; i++) {
		if( regexp[i] == '\\') i++;
		else if( regexp[i] == '[') {
			while( regexp[i] != ']') i++;
		}
		len++;
	}
	return len;
}
