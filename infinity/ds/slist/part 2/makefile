CC=gcc
# CFLAGS=-ansi -pedantic-errors -Wall -Wextra -g -I.
CFLAGS=-ansi -pedantic-errors -Wall -Wextra -g -I.
DEPS = slist.h
TARGET = a.out
ARCHIVE = lib.ar
OBJ = slist_ex.o
AR=ar

%.o: %.c $(DEPS)
	
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^

	ar r $(ARCHIVE) $(OBJ)
	rm *.o

.PHONY: clean

clean:
	rm *.out
	rm *.ar


.PHONY: run

run:
	./$(TARGET)

