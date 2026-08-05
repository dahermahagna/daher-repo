CC=gcc
CFLAGS=-ansi -pedantic-errors -Wall -Wextra -g -I.
DEPS = calc.h
TARGET = a.out
ARCHIVE = calc.ar
OBJ = calc_test.o calc.o ../../stack/stack.o ../parser/parser.o
AR=ar

%%.o: %%.c $(DEPS)

	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ -lm

	ar r $(ARCHIVE) $(OBJ)
	rm *.o
	rm ../parser/*.o
	rm ../../stack/*.o


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