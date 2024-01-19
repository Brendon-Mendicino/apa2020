
#ifndef QUOATAZIONE_H
#define QUOATAZIONE_H


#include <stdio.h>

#include "mytime.h"


#define POST_ORDER 1
#define PRE_ORDER  2
#define IN_ORDER   3


typedef struct {
	mytime_t time; /* Si considera solo la data.  */
	float value;
	int scambi;
} quotazione_t;

typedef struct quot_bst *Qbst;




quotazione_t QuotNull( void);

int QuotCompare( quotazione_t a, quotazione_t b);

void QuotPrint( FILE *output, quotazione_t quot);



Qbst BstInit( void);

void BstFree( Qbst bst);

int BstGetNodes( Qbst bst);

void BstInsert( Qbst bst, quotazione_t quot);

quotazione_t BstSearchByDate( Qbst bst, mytime_t date);

quotazione_t BstSelect( Qbst bst, int n);

quotazione_t BstSearchMinBetweenDate( Qbst bst, mytime_t data1, mytime_t data2);

quotazione_t BstSearchMaxBetweenDate( Qbst bst, mytime_t data1, mytime_t data2);

void BstBalance( Qbst bst, int soglia);

void BstPrint( FILE *output, Qbst bst, int order);

#endif  // QUOATAZIONE_H
