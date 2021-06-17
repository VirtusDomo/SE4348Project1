#!/usr/bin/make -f
#
#Filename:	Makefile
#Date:		06/15/2021
#Author:	James Anyabine
#Email: 	joa170000@utdallas.edu
#Version:	1.0
#Copyright:	2021, All Rights Reserved
#
#Description:
#	This is the general use Makefile for Project 1 of SE 4348. It will be adjusted to accomodate for whatever
#	functions that are necessary for the project
#

#Flags for the C++ implicit Rules
CXX = g++
CXXFLAGS = -Wall

#Project Name of Executable
PROJECTNAME = SE4348.Program1

EXEC = program1

SRCS = $(wildcard *.cc)
HEADERS = $(wildcard *.h)
OBJS = $(patsubst %.cc,%.o,$(SRCS))

all: $(EXEC)

clean:
	rm -f $(OBJS) *.d *~ \#* *.out $(EXEC)

Makefile: $(SRCS:.cc=.d)

%.d:%.cc
	@echo Updating .d Dependency File
	@set -e; rm -f $@; \
	$(CXX) -MM $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

$(EXEC): $(OBJS)
	@echo Compiling Program...
	$(CXX) -o $(EXEC) $(OBJS)
	@echo Compilation completed successfully!

backup:
	@make clean 
	@mkdir -p ~/backups; chmod 700 ~/backups
	@$(eval CURDIRNAME := $(shell basename "`pwd`"))
	@$(eval MKBKUPNAME := ~/backups/$(PROJECTNAME)-$(shell date +'%Y.%m.%d-%H:%M:%S').tar.gz)
	@echo
	@echo Writing Backup file to: $(MKBKUPNAME)
	@echo 
	@-tar zcfv $(MKBKUPNAME) ../$(CURDIRNAME) 2> /dev/null
	@chmod 600 $(MKBKUPNAME)
	@echo 
	@echo Done!

-include $(SRCS:.cc=.d)










