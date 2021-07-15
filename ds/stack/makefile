CC=gcc
# CFLAGS=-ansi -pedantic-errors -Wall -Wextra -g -I.
CFLAGS=-ansi -pedantic-errors -Wall -Wextra -g -I.
DEPS = stack.h
TARGET = a.out
ARCHIVE = ../lib/stack.ar
OBJ = stack.o ../test/stack_test.o
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




