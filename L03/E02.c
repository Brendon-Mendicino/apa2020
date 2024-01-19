/* Brendon Mendicino, 271784 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_BITS 128


typedef enum {false, true} bool;

void stampaCodifica( void *p, int size, bool bigEndian);
void trasf_bit( unsigned char a, unsigned char *bit_buffer, int pos);
void stampa( unsigned char *bit_buffer, int size, int exp);

int main( void)
{
	bool bigEndian;
	float af = 0.0;
	double ad = 0.0;
	long double ald = 0.0;
	int size_af = 0, size_ad = 0, size_ald = 0;

	/* bigEndian test */
	int test = 1;
	char *a = (char *)(&test);
	if( *a == 1) bigEndian = false;
	else bigEndian = true;
	printf( "%d %d\n", *a, *(a+sizeof(int)-1));


	size_af = sizeof(af);
	size_ad = sizeof(ad);
	size_ald = sizeof(ald);

	printf( "Size of float: %dbyte, %dbit\n", size_af, 8*size_af);
	printf( "Size of double: %dbyte, %dbit\n", size_ad, 8*size_ad);
	printf( "Size of long double: %dbyte, %dbit\n", size_ald, 8*size_ald);

	printf( "Inserire un: float, double, long double: ");
	scanf( "%f%lf%Lf", &af, &ad, &ald);

	stampaCodifica( (void *)&af, sizeof(af), bigEndian);
	stampaCodifica( (void *)&ad, sizeof(ad), bigEndian);
	stampaCodifica( (void *)&ald, sizeof(ald), bigEndian);

	return 0;
}


void stampaCodifica( void *p, int size, bool bigEndian)
{
	/* I long double sono rappresentati su 10 byte */
	if( size == sizeof(long double)) size = 10;

	int byte_pos = size;
	/* bit_buffer conterra' i bit con posizione [0] il MSB */
	unsigned char bit_buffer[MAX_BITS] = {0};
	unsigned char *a = (unsigned char *)p;

	if( bigEndian == false) {
		/* parto dal LSB */
		for( int i = 0; i < size; i++) {
			trasf_bit( a[i], bit_buffer, byte_pos);
			byte_pos--;
		}
	}
	else {
		/* parto dal LSB */
		for( int i = size-1; i >= 0; i--) {
			trasf_bit( a[i], bit_buffer, byte_pos);
			byte_pos--;
		}
	}

	int exp = 0;
	switch (size) {
		case sizeof(float):
			exp = 8;
			printf( "float:");
			stampa( bit_buffer, size, exp);
			break;
		case sizeof(double):
			exp = 11;
			printf( "double:");
			stampa( bit_buffer, size, exp);
			break;
		//case sizeof(long double):
		case 10:
			exp = 15;
			printf( "long double:");
			stampa( bit_buffer, size, exp);
			break;
	}
}

void trasf_bit( unsigned char a, unsigned char *bit_buffer, int pos) 
{
	int x = 0;
	while( a / 2 != 0) {
		bit_buffer[ (pos*8)-1 - x] = a%2;
		if( a % 2 == 0) a = a/2;
		else a = (a-1)/2;
		x++;
		
		if( a/2 == 0) bit_buffer[ (pos*8)-1 - x] = 1;
	}
}

void stampa( unsigned char *bit_buffer, int size, int exp)
{
	printf( "s%d e", bit_buffer[0]);
	for( int i = 1; i < size*8; i++) {
		if( i == 1+exp) printf( " m");
		printf( "%d", bit_buffer[i]);
	}
	printf( "\n");
}
