
ERR_F = -Wall -Wextra
DBG = -g

test: main.o elementi.o
	gcc $(ERR_F) $(DBG) main.o elementi.o -o test

main.o: main.c elementi.h
	gcc $(ERR_F) $(DBG) -c main.c -o main.o

elementi.o: elementi.c elementi.h
	gcc $(ERR_F) $(DBG) -c elementi.c -o elementi.o

clear:
	rm *.o test

