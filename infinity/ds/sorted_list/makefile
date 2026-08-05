CC=gcc
CFLAGS=-ansi -pedantic-errors -Wall -Wextra -g -I.
DEPS = sorted_list.h
TARGET = a.out
ARCHIVE = ../lib/sorted_list.ar
OBJ = ../test/sorted_list_test.o sorted_list.o ../dlist/dlist.o
AR=ar

%%.o: %%.c $(DEPS)

	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^

	ar r $(ARCHIVE) $(OBJ)
	rm *.o
	rm ../test/*.o
	rm ../dlist/*.o

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

.PHONY: ndebug
ndebug:
	gcc -ansi -pedantic-errors -Wall -Wextra -DNDEBUG sorted_list.c sorted_list_test.c ../dlist/dlist.c
	./a.out

