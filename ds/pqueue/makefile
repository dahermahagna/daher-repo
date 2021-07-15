CC=gcc
CFLAGS=-ansi -pedantic-errors -Wall -Wextra -g -I.
DEPS = pqueue.h
TARGET = a.out
ARCHIVE = ../lib/pqueue.ar
OBJ = ../test/pqueue_test.o heap_PQ.o ../heap/heap.o  ../dynamic_vector/vector.o
AR=ar

%%.o: %%.c $(DEPS)

	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^

	ar r $(ARCHIVE) $(OBJ)
	rm *.o
	rm ../test/*.o
	rm ../dynamic_vector/*.o
	rm ../heap/*.o

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