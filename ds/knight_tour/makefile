CC=gcc
CFLAGS=-ansi -pedantic-errors -Wall -Wextra -g -I.
DEPS = vector.h
TARGET = a.out
ARCHIVE = ../lib/vector.ar
OBJ = ../dynamic_vector/vector.o ../bit_array/bit_array.o knight_tour.o 
AR=ar
#compiler defs
ifdef PRT
CFLAGS += -D$(PRT)
endif
%.o: %.c $(DEPS)
	
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ 

	ar r $(ARCHIVE) $(OBJ)
	rm *.o
	rm ../dynamic_vector/*.o

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



