CC=gcc
CFLAGS=-ansi -pedantic-errors -Wall -Wextra -g -I.
DEPS = recursion.h
TARGET = a.out
ARCHIVE = recursion.ar
OBJ = recursion.o recursion_test.o ../../ds/stack/stack.o
AR=ar

%.o: %.c $(DEPS)
	
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^

	ar r $(ARCHIVE) $(OBJ)
	rm *.o
	rm ../../ds/stack/*.o
	
	

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