CC=g++
CFLAGS=-std=c++98 -ansi -pedantic-errors -Wall -Wextra -g -I.
SFLAGS=-lboost_thread -lboost_system -lboost_chrono -lpthread
DEPS = singleton_impl.hpp
TARGET = a.out
ARCHIVE = sin.ar
OBJ =  singleton_test.o
AR=ar

%%.o: %%.c $(DEPS)

	$(CC) -c -o $@ $< $(CFLAGS) $(SFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(SFLAGS)

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