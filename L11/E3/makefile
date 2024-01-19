ERR_FLAG = -Wall -Wextra
DBG = -g


test: main.o symbol_t.o graph.o
	gcc $(ERR_FLAG) $(DBG) main.o symbol_t.o graph.o -o test

main.o: main.c graph.h
	gcc $(ERR_FLAG) $(DBG) -c main.c -o main.o

graph.o: graph.c graph.h symbol_t.h
	gcc $(ERR_FLAG) $(DBG) -c graph.c -o graph.o

symbol_t.o: symbol_t.c symbol_t.h
	gcc $(ERR_FLAG) $(DBG) -c symbol_t.c -o symbol_t.o

clear:
	rm *.o test
