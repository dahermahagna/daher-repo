CC=gcc
CFLAGS=-ansi -pedantic-errors -Wall -Wextra -g -I.
DEPS = ppserver.h
TARGETS = server tcpclient udpclient
ARCHIVE = ppserver.ar
OBJ = ppserver.o server_test.o tcpc.o udpc.o 
AR=ar


%%.o: %%.c $(DEPS)

	$(CC) -c -o $@ $< $(CFLAGS) 

all: server tcpclient udpclient
server: ppserver.o server_test.o
	$(CC) -o $@ $^ -lm
tcpclient: tcpc.o
	$(CC) -o $@ $^ -lm
udpclient: udpc.o
	$(CC) -o $@ $^ -lm

	ar r $(ARCHIVE) $(OBJ)
	rm *.o

.PHONY: clean
clean:
	rm server
	rm *.ar

.PHONY: run
run:
	./server

.PHONY: vlg
vlg:
	valgrind --leak-check=yes --track-origins=yes ./$(TARGET)