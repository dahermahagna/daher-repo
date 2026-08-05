CC=g++
CFLAGS= -std=c++98 -ansi -pedantic-errors -Wall -Wextra -g -fPIC 
SFLAGS=-shared
INC_DIR_PATH = -I../include -I../utils
DEPS =nbdproxy.hpp
TARGET = libexp.so
ARCHIVE = nbdproxy.ar
OBJS = nbdproxy.o


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


