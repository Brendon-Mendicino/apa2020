
ERR_FLAGS = -Wall -Wextra

test: gdr.o pg.o pgList.o invArray.o
	gcc $(ERR_FLAGS) -g gdr.o pgList.o pg.o equipArray.o invArray.o inv.o -o test

gdr.o: gdr.c
	gcc $(ERR_FLAGS) -g -c gdr.c -o gdr.o

pgList.o: pgList.h pgList.c
	gcc $(ERR_FLAGS) -g -c pgList.c -o pgList.o

pg.o: pg.h pg.c equipArray.o invArray.o inv.o
	gcc $(ERR_FLAGS) -g -c pg.c -o pg.o

equipArray.o: equipArray.h equipArray.c invArray.o
	gcc $(ERR_FLAGS) -g -c equipArray.c -o equipArray.o

invArray.o: invArray.h invArray.c inv.o
	gcc $(ERR_FLAGS) -g -c invArray.c -o invArray.o

inv.o: inv.h inv.c
	gcc $(ERR_FLAGS) -g -c inv.c

clear:
	rm *.o
