
#include <stdio.h>
#include <string.h>
adsfasdf
typedef struct cane Abba;
typedef struct cane{
	char a[20];
	int b;
	char c[20];
	Abba *cazz;
} Abba;

int main(void)
{
	Abba h = { "bella", 10, "tu"};
	Abba f = {{0}};

	f = h;
	f = { "bruh", 9, "ok?"};

	printf( "%s%d%s\n", h.a, h.b, h.c);
	printf( "%s%d%s\n", f.a, f.b, f.c);
	return 0;
}
