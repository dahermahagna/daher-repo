CC=gcc
CFLAGS=-ansi -pedantic-errors -Wall -Wextra -g -I.
DEPS = vector.h
TARGET = a.out
ARCHIVE = ../lib/vector.ar
OBJ = vector.o bit_array.o knight_tour.o 
AR=ar

%.o: %.c $(DEPS)
	
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^


	rm *.o

.PHONY: clean
clean:
	rm *.out


.PHONY: run
run:
	./$(TARGET)

.PHONY: vlg
vlg:
	valgrind --leak-check=yes --track-origins=yes ./$(TARGET)

.PHONY: cgdb
cgdb:
	cgdb ./$(TARGET)



