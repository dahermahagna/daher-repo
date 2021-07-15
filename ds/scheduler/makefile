CC=gcc
CFLAGS=-ansi -pedantic-errors -Wall -Wextra -g -I.
DEPS = scheduler.h
TARGET = a.out
ARCHIVE = ../lib/scheduler.ar
OBJ = ../test/scheduler_test.o scheduler.o ../uid/uid.o ../pqueue/pqueue.o ../task/task.o ../sorted_list/sorted_list.o ../dlist/dlist.o
AR=ar

%%.o: %%.c $(DEPS)

	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^

	ar r $(ARCHIVE) $(OBJ)
	rm *.o
	rm ../uid/*.o
	rm ../test/*.o
	rm ../pqueue/*.o
	rm ../task/*.o
	rm ../sorted_list/*.o
	rm ../dlist/*.o
	

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