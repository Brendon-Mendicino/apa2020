#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "mytime.h"
#include "quotazione.h"
#include "titolo.h"


#define DBG

typedef enum {
	opt_min = 0,
	search_quot,
	search_min_max_in_dates,
	search_min_max_abs,
	balance,
	exit_opt,
	opt_max
} option_e;



void flushStdin( void)
{
	char a;
	while( (a = getchar()) != '\n' && a != EOF);
}

void readStringFromStdin( char *str)
{
	static char buff[300];
	fgets( buff, 300, stdin);
	sscanf( buff, "%s", str);

	if( strstr( buff, "\n") == NULL)
		flushStdin();
}

int searchTitolo( TList list, Titolo *titolo)
{
	char code[MAX_CODE];
	while(1) {
		printf( "\nRicerca un titolo.\n");
		printf( "Type [exit] per uscire.\n");
		printf( "Inserisci il codice: ");
		fflush( stdout);

		readStringFromStdin( code);

		if( strcmp( "exit", code) == 0)
			return 0;

		*titolo = ListSearchByCode( list, code);
		if( !TitoloCheckNull( *titolo))
			return 1;
		else
			fprintf( stderr, "No match found!\n");
	}
}


void printPrompt( void)
{
	printf( " (1) ~> Ricerca quotazione.\n");
	printf( " (2) ~> Cerca quotazione minina e massima in un intervallo.\n");
	printf( " (3) ~> Cerca quotazione minima e massima assulute.\n");
	printf( " (4) ~> Bilalcia l'albero delle quotazioni.\n");
	printf( " (5) ~> Exit.\n");
	printf( ">>> ");
	fflush( stdout);
}

int readOption( option_e *opt)
{
	char buff[10];
	int x;
	readStringFromStdin( buff);
	sscanf( buff, "%d", &x);
	if( x <= opt_min || x >= opt_max)
		return 0;
	*opt = x;
	return 1;
}



int main( void)
{
	TList list;
	Titolo titolo_sel;
	option_e opt;
	int loop = 1;

	list = ListInit();

	ListLoad( "F1.txt", list);
	ListLoad( "F2.txt", list);
	ListLoad( "F3.txt", list);

#ifdef DBG
	ListPrint( stdout, list);
#endif

	while( loop) {
		if( searchTitolo( list, &titolo_sel) == 0)
			break;
		printPrompt();
		while( !readOption( &opt)) {
			fprintf( stderr, "Opzione non valida!\n");
			printPrompt();
		}

		if( opt == search_quot)
		{
			mytime_t date;
			quotazione_t quot;
			printf( "\nInserisci la data [aaaa/mm/dd]: ");
			TIMEreadDate( stdin, &date);
			quot = TitoloSearchQuot( titolo_sel, date);

			if( QuotCompare( quot, QuotNull()) != 0)
				QuotPrint( stdout, quot);
			else
				fprintf( stderr, "No match found!\n");
		}
		else if( opt == search_min_max_in_dates)
		{
			mytime_t data1, data2;
			quotazione_t min , max;
			printf( "\nInserisci l'intervallo in cui vuoi cercare [aaaa/mm/gg]: ");

			TIMEreadDate( stdin, &data1);
			TIMEreadDate( stdin, &data2);
			min = TitoloSearchMinBeetwenDate( titolo_sel, data1, data2);
			max = TitoloSearchMaxBeetwenDate( titolo_sel, data1, data2);

			printf( "Minimo:\n");
			QuotPrint( stdout, min);
			printf( "Massimo:\n");
			QuotPrint( stdout, max);
		}
		else if( opt == search_min_max_abs)
		{
			mytime_t data1 = TIMEdateMin();
			mytime_t data2 = TIMEdateMax();
			quotazione_t min , max;

			min = TitoloSearchMinBeetwenDate( titolo_sel, data1, data2);
			max = TitoloSearchMaxBeetwenDate( titolo_sel, data1, data2);

			printf( "Minimo:\n");
			QuotPrint( stdout, min);
			printf( "Massimo:\n");
			QuotPrint( stdout, max);
		}
		else if( opt == balance)
		{
			int soglia;

			printf( "Scegli la soglia per bilanciare l'albera delle quotazioni: ");
			scanf( "%d", &soglia);
			flushStdin();

			TitoloBalceQuotTree( titolo_sel, soglia);
		}
		else if( opt == exit_opt)
		{
			loop = 0;
		}

#		ifdef DBG
		ListPrint( stdout, list);
#		endif
	}

	ListFree( list);
	return 0;
}
