COMPILER_DIR	=	TinyCompiler/TinyCompiler 
MACHINE_DIR		=	TinyMachine/TinyMachine

VPATH	=	$(COMPILER_DIR) $(MACHINE_DIR)
CC		=	gcc
CFLAGS	=	-c -Wall
RM 		=	rm -f

.PHONY: all
all:	TinyCompiler.out TinyMachine.out

TinyCompiler.out:	tc_main.o cgen.o analyze.o parse.o scan.o symtab.o code.o util.o
	$(CC) $^ -o $@
	$(RM) $^

tc_main.o:		tc_main.c cgen.h analyze.h parse.h scan.h
	$(CC) $(CFLAGS) $<

cgen.o:			cgen.c code.h symtab.h
	$(CC) $(CFLAGS) $<

code.o:			code.c globals.h
	$(CC) $(CFLAGS) $<

analyze.o:		analyze.c symtab.h globals.h 
	$(CC) $(CFLAGS) $<

symtab.o:		symtab.c
	$(CC) $(CFLAGS) $<

parse.o:		parse.c scan.h globals.h util.h
	$(CC) $(CFLAGS) $<

scan.o:			scan.c globals.h util.h
	$(CC) $(CFLAGS) $<

util.o:			util.c
	$(CC) $(CFLAGS) $<



TinyMachine.out:	tm_main.o tm.o
	$(CC) $^ -o $@
	$(RM) $^

tm_main.o:		tm_main.c tm.h
	$(CC) $(CFLAGS) $<

tm.o:			tm.c
	$(CC) $(CFLAGS) $<



.PHONY:	clean
clean:
	$(RM) *.o *.out






















