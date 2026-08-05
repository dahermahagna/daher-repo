CC=gcc
CFLAGS=-ansi -pedantic-errors -Wall -Wextra -g -I.
DEPS = queue.h ../include/slist.h
TARGET = a.out
ARCHIVE = ../lib/queue.ar
OBJ = queue.o ../test/queue_test.o ../slist/slist.o
AR=ar

%.o: %.c $(DEPS)
	
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^

	ar r $(ARCHIVE) $(OBJ)
	rm *.o
	rm ../test/*.o
	rm ../slist/slist.o

.PHONY: clean
clean:
	rm *.out
	rm *.ar


.PHONY: run
run:
	./$(TARGET)

