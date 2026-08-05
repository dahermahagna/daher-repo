MKDIR   := @mkdir -p
RMDIR   := rm -r 
CC      := gcc

MOTHERDIR := ../
#debug is default. can be changed with - MODE=release - in terminal
MODE = debug
ifeq ($(MODE), debug)
DBFOLDER =debug/
DBFLAGS =  -g
EXECPATH = ../test/
else
DBFOLDER =
DBFLAGS = -DNDEBUG -O3 
EXECPATH = ./
endif
#folder names
LIB     := ../lib/$(DBFOLDER)
OBJ     := ../obj/$(DBFOLDER)
INCLUDE := ../include/
UTILS	:= ../utils/
TEST 	:= ../test/
SRC     := ./

TARGET = $(EXECPATH)$(PROJ)_$(MODE).out
ARCHIVE = ar.lib
CFLAGS 	:= -fPIC -ansi -pedantic-errors -Wall -Wextra $(DBFLAGS) -I$(INCLUDE)
LDLIBS  := -shared 
LDFLAGS = -Wl,-rpath,=$(LIB) -L$(LIB) -l$(PROJ)

DIRS := $(sort $(filter-out \, $(dir $(wildcard ../*/))))
DIRS := $(filter-out ../obj/ ../lib/ ../ $(LIB) $(OBJ) $(INCLUDE) $(UTILS) $(TEST), $(DIRS))

PROJ 	:= $(notdir $(CURDIR))
TESTCFILE	:= ../$(PROJ)/$(PROJ)_test.c
REG		:= ../$(PROJ)/$(PROJ).c

LIBS 	:= $(LIB)lib$(PROJ).so
OBJS    := $(OBJ)$(PROJ)_test.o

DEPS := $(sort $(strip $(notdir $(basename $(filter-out \, $(shell gcc -MM $(REG) -I$(INCLUDE)))))))
DEPS += $(foreach x, $(DEPS), $(notdir $(basename $(filter-out \, $(shell gcc -MM ../$(x)/$(x).c -I$(INCLUDE))))))
DEPS := $(sort $(strip $(DEPS)))
OFILES	:= $(foreach x, $(DEPS), $(OBJ)$(x).o)
CFILES	:= $(foreach x, $(DEPS), ../$(x)/$(x).c)


.PHONY: all_dirs clean-all-dirs all run clean archive print test help

all: $(TARGET)

$(OFILES): $(CFILES)
	$(shell for f in $(DEPS); do \
	$(CC) -c ../$$f/$$f.c -o $(OBJ)$$f.o $(CFLAGS); done)

$(LIBS): $(OFILES) | $(LIB)
	$(CC) $(LDLIBS) $^ -o $@ $(CFLAGS)

$(OBJS): $(TESTCFILE) | $(OBJ)
	$(CC) -c $^ -o $@ $(CFLAGS)

$(TARGET): $(OBJS) $(LIBS) 
	$(CC) $^ $(LDFLAGS) -o $@ 

$(OBJ):
	$(MKDIR) $@

$(LIB):
	$(MKDIR) $@

all-dirs:
	$(shell for d in $(DIRS); do \
	cp $(UTILS)makefile $$d ; \
	$(MAKE) -C $$d MODE=$(MODE);\
	done)

clean-all-dirs:
	$(shell for d in $(DIRS); do \
	$(MAKE) clean MODE=$(MODE) -C $$d ;\
	done)

print:
	@echo $(EXECPATH) $(DBFOLDER) $(PROJ) $(TEST) Reg= $(REG) OBJS= $(OBJS) Dirs = $(DIRS) Deps = $(DEPS) CFlies= $(CFILES) Libs = $(LIBS) PROJ= $(PROJ) ofiles = $(OFILES)

test: 
	$(TARGET)

run: 
	$(TARGET)

clean:
#	$(RMDIR) $(OBJ) 
	rm -f $(TARGET)
	rm -f $(OFILES)
	rm -f $(OBJS)
	rm -f $(LIBS)

archive:
	ar r $(ARCHIVE) $(OBJ)

help:
	@echo make  					- will compile in default debug mode. 
	@echo make MODE=release 		- compile in release mode
	@echo make all-dirs 			- copies makefile from utils library to all directories and compile them
	@echo make all-dirs MODE=release - copies makefile from utils to all dirs and compile them in release
	@echo make test  //or make run	- runs exec of test mode.  
	@echo make test MODE=release  	- runs it in realse mode
	@echo make clean				- delete all o, os, out files created. 
	@echo make clean MODE=release	- delete all o, os, out files created in release mode. 
	@echo make clean-all-dirs //optional// MODE=release	- delete all o, os, out files in all dirs. 