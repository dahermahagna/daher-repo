CC=g++
CFLAGS=-std=c++98 -ansi -pedantic-errors -Wall -Wextra -g
SFLAGS=-lboost_thread -lboost_system -lpthread
INC_DIR_PATH = -I../utils
DEPS =factory.hpp
TARGET = a.out
ARCHIVE = fac.ar
OBJS = factory_test.o


%.o: %.cpp $(DEPS)

	$(CC) -c -o $@ $< $(CFLAGS) $(INC_DIR_PATH)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

	ar r $(ARCHIVE) $(OBJS)
	rm *.o

.PHONY: clean
clean:
	rm *.out
	rm *.ar

.PHONY: wd
wd:
	make -C include/wd_app
.PHONY: run
run:
	./$(TARGET)

.PHONY: vlg
vlg:
	valgrind --leak-check=yes --track-origins=yes ./$(TARGET)


