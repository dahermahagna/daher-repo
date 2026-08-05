CC=g++
CFLAGS=-std=c++98 -ansi -pedantic-errors -Wall -Wextra -g -I.
SFLAGS=-lboost_thread -lboost_system -lboost_chrono -lpthread
DEPS = thread_pool_impl.hpp
TARGET = a.out
ARCHIVE = TP.ar
OBJ =  thread_pool_test.o thread_pool.o
AR=ar

%%.o: %%.c $(DEPS)

	$(CC) -c -o $@ $< $(CFLAGS) $(SFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(SFLAGS)

	ar r $(ARCHIVE) $(OBJ)



.PHONY: clean
clean:
	rm *.out
	rm *.ar
	rm *.o

.PHONY: run
run:
	./$(TARGET)

.PHONY: vlg
vlg:
	valgrind --leak-check=yes --track-origins=yes ./$(TARGET)