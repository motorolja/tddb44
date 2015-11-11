CC	=	g++
CFLAGS	=	-std=c++11 -ggdb3 -Wall -Woverloaded-virtual -Wno-write-strings -Wno-unused-but-set-variable
#CC	=	CC
#CFLAGS	=	-g +p +w
GCFLAGS =	-std=c++11 -g -Wall -Wno-unused-function -Wno-unused-variable
LDFLAGS =
DPFLAGS =	-MM

SOURCES =	error.cc scanner.cc symtab.cc symbol.cc symtabtest.cc
HEADERS =	error.hh scanner.hh symtab.hh
OBJECTS =	$(SOURCES:%.cc=%.o)
OUTFILE =	symtab

DPFILE  =	Makefile.dependencies

all : $(OUTFILE)

$(OUTFILE) : $(OBJECTS)
	$(CC) -o $(OUTFILE) $(OBJECTS) $(LDFLAGS)

scanner.cc : scanner.l
	flex scanner.l

scanner.o : scanner.cc
	$(CC) $(GCFLAGS) -c $<

.cc.o: $(DPFILE)
	$(CC) $(CFLAGS) -c $<

clean :
	rm -f $(OBJECTS) $(OUTFILE) core *~ scanner.cc $(DPFILE)
	touch $(DPFILE)


$(DPFILE) depend : $(SOURCES) $(HEADERS)
	$(CC) $(DPFLAGS) $(CFLAGS) $(SOURCES) > $(DPFILE)

include $(DPFILE)
