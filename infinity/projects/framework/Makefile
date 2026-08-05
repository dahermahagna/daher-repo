CC=g++
CFLAGS=-std=c++98 -ansi -pedantic-errors -Wall -Wextra -g
SFLAGS=-lboost_thread -lboost_system -lpthread
INC_DIR_PATH = -I../include -I../utils
DEPS =framework.hpp
TARGET = a.out
ARCHIVE = framework.ar
OBJS = framework_test.o


%.o: %.cpp $(DEPS)

	$(CC) -c -o $@ $< $(CFLAGS) $(SFLAGS) $(INC_DIR_PATH)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(SFLAGS)

	ar r $(ARCHIVE) $(OBJS)

.PHONY: clean
clean:
	rm *.o
	rm *.out
	rm *.ar

.PHONY: run
run:
	./$(TARGET)

.PHONY: vlg
vlg:
	valgrind --leak-check=yes --track-origins=yes ./$(TARGET)


