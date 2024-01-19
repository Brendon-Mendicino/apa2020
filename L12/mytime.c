#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "mytime.h"


static void flushStdin( void)
{
	char a;
	while( (a = getchar()) != '\n' && a != EOF);
}

static void readString( char *str)
{
	static char buff[300];
	fgets( buff, 300, stdin);
	sscanf( buff, "%s", str);
	
	if( strstr( buff, "\n") == NULL)
		flushStdin();
}


mytime_t TIMEnull( void)
{
	mytime_t time;
	time.year = 0;
	time.month = 0;
	time.day = 0;
	time.h = 0;
	time.m = 0;
	return time;
}

mytime_t TIMEdateMin( void)
{
	mytime_t time;
	time.year = -1;
	time.month = -1;
	time.month = -1;
	return time;
}

mytime_t TIMEdateMax( void)
{
	mytime_t time;
	time.year = INT_MAX;
	time.month = INT_MAX;
	time.day = INT_MAX;
	return time;
}



void TIMEdatePrint( FILE *output, mytime_t time)
{
	fprintf( output, "%02d/%02d/%d", time.day, time.month, time.year);
}

int TIMEdatecmp( mytime_t a, mytime_t b)
{
	if( a.year != b.year)
		return (a.year - b.year);

	if( a.month != b.month)
		return (a.month - b.month);

	if( a.day != b.day)
		return (a.day - b.day);
	return 0;
}

int TIMEcompare( mytime_t a, mytime_t b)
{
	if( a.year != b.year)
		return (a.year - b.year);

	if( a.month != b.month)
		return (a.month - b.month);

	if( a.day != b.day)
		return (a.day - b.day);

	if( a.h != b.h)
		return (a.h - b.h);

	if( a.m != b.m)
		return (a.m - a.m);

	return 0;
}

void TIMEreadDate( FILE *input, mytime_t *time)
{
	/* Forma:
	 * "aaaa/mm/gg hh:mm"
	 */
	if( input == stdin) {
		char buff[DATA_STR];
		readString( buff);
		sscanf( buff, "%d/%d/%d", &time->year, &time->month, &time->day);
		return;
	}
	fscanf( input, "%d/%d/%d", &time->year, &time->month, &time->day);
}

void TIMEreadFromString( char *str, mytime_t *time)
{
	/* Forma:
	 * "aaaa/mm/gg hh:mm"
	 */
	sscanf( str, "%d/%d/%d %d:%d", &time->year, &time->month, &time->day,
			&time->h, &time->m);
}
