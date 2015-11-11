CC	=	g++
CFLAGS	=	-std=c++11 -ggdb3 -Wall -Woverloaded-virtual -pedantic
#CC	=	CC
#CFLAGS	=	-g +p +w
GCFLAGS =	-std=c++11 -g -Wall -Wno-unused-function -Wno-unused-variable
LDFLAGS =
DPFLAGS =	-MM

BASESRC =	symbol.cc symtab.cc ast.cc semantic.cc optimize.cc quads.cc codegen.cc error.cc main.cc
SOURCES =	$(BASESRC) parser.cc scanner.cc
BASEHDR =	symtab.hh error.hh ast.hh semantic.hh optimize.hh quads.hh codegen.hh
HEADERS =	$(BASEHDR) parser.hh
OBJECTS =	$(SOURCES:%.cc=%.o)
OUTFILE =	compiler

DPFILE  =	Makefile.dependencies

all : $(OUTFILE)

$(OUTFILE) : $(OBJECTS)
	$(CC) -o $(OUTFILE) $(OBJECTS) $(LDFLAGS)

foo : foo.cc
	$(CC) $(CFLAGS) -o foo

parser.cc parser.hh : parser.y Makefile
	bison --defines --output-file=parser.cc parser.y

scanner.cc : scanner.l
	flex scanner.l

scanner.o : scanner.cc
	$(CC) $(GCFLAGS) -c $<

parser.o : parser.cc
	$(CC) $(GCFLAGS) -c $<

.cc.o: $(DPFILE)
	$(CC) $(CFLAGS) -c $<

clean :
	rm -f $(OBJECTS) $(OUTFILE) core *~ scanner.cc parser.cc parser.hh parser.cc.output $(DPFILE)
	touch $(DPFILE)


$(DPFILE) depend : $(BASESRC) $(HEADERS) $(SOURCES)
	$(CC) $(DPFLAGS) $(CFLAGS) $(BASESRC) > $(DPFILE)

include $(DPFILE)
