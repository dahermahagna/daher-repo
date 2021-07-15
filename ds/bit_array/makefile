CC=gcc
# CFLAGS=-ansi -pedantic-errors -Wall -Wextra -g -I.
CFLAGS=-ansi -pedantic-errors -Wall -Wextra -g -I.
DEPS = bit_array.h
TARGET = a.out
ARCHIVE = ../lib/bit_array.ar
OBJ = bit_array.o ../test/bit_array_test.o
AR=ar

%.o: %.c $(DEPS)
	
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^

	ar r $(ARCHIVE) $(OBJ)
	rm *.o
	rm ../test/*.o

.PHONY: clean
clean:
	rm *.out
	rm *.ar

.PHONY: run
run:
	./$(TARGET)

.PHONY: vlg
vlg:
	valgrind --leak-check=yes --track-origins=yes ./$(TARGET)

.PHONY: cgdb
cgdb:
	cgdb ./$(TARGET)




