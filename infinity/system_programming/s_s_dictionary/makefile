CC=gcc
CFLAGS=-ansi -pedantic-errors -Wall -Wextra -g -I. 
TARGET = s_s_dictionary.out
ARCHIVE = s_s_dictionary.ar
OBJ = s_s_dictionary.o
AR=ar

%%.o: %%.c $(DEPS)

	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ -lpthread

	ar r $(ARCHIVE) $(OBJ)
	rm *.o

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