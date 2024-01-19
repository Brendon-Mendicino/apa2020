
ERR_FLAG = -Wall -Wextra
DBG = -g

test: main.o mytime.o quotazione.o titolo.o
	gcc $(ERR_FLAG) $(DBG) main.o mytime.o quotazione.o titolo.o -o test

main.o: main.c titolo.h
	gcc $(ERR_FLAG) $(DBG) -c main.c -o main.o

titolo.o: titolo.c titolo.h quotazione.h mytime.h
	gcc $(ERR_FLAG) $(DBG) -c titolo.c -o titolo.o

quotazione.o: quotazione.c quotazione.h mytime.h
	gcc $(ERR_FLAG) $(DBG) -c quotazione.c -o quotazione.o

mytime.o: mytime.c mytime.h
	gcc $(ERR_FLAG) $(DBG) -c mytime.c -o mytime.o

clear:
	rm *.o test
