

test: main.o character.o items.o
	gcc -Wall -Wextra -g main.o character.o items.o -o test

main.o: main.c defines.h items.h character.h
	gcc -Wall -Wextra -g -c main.c -o main.o

character.o: character.c defines.h items.h character.h
	gcc -Wall -Wextra -g -c character.c -o character.o

items.o: items.c defines.h items.h
	gcc -Wall -Wextra -g -c items.c -o items.o

clean:
	rm *.o test
