
#ifndef MYTIME_H
#define MYTIME_H


#include <stdio.h>

#define DATA_STR  15
#define CLOCK_STR 10

typedef struct {
	int year;
	int month;
	int day;
	int h;
	int m;
} mytime_t;



mytime_t TIMEnull( void);

mytime_t TIMEdateMin( void);

mytime_t TIMEdateMax( void);

int TIMEdatecmp( mytime_t a, mytime_t b);

void TIMEdatePrint( FILE *output, mytime_t time);

int TIMEcompare( mytime_t a, mytime_t b);

void TIMEreadDate( FILE *input, mytime_t *time);

void TIMEreadFromString( char *str, mytime_t *time);

#endif  // MYTIME_H
