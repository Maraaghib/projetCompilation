SRCBISON = ppascal.y
SRCFLEX = $(SRCBISON:.y=.l)  
CBISON = $(SRCBISON:.y=bison.c)
HBISON = $(CBISON:.c=.h)
CFLEX = $(SRCFLEX:.l=flex.c)
CDEP = util.c environ.c bilquad.c tradpp2c3a.c
ODEP = $(CDEP:.c=.o)
INCLUDE = -I include/
EXE = ppascal interppC3A
CFLAGS = -g -lfl -Wunused-variable 

all : $(EXE)

ppascal :  $(CBISON) $(CFLEX) $(ODEP)
	gcc -o $@ $(INCLUDE) $^ $(CFLAGS)

$(CBISON): src/$(SRCBISON)
	bison -d -o$@ $<

$(CFLEX): src/$(SRCFLEX)
	flex -o $@ $<

util.o : src/util.c include/util.h environ.o
	gcc -c -std=c99 $(INCLUDE) -g -o $@ $<

environ.o : src/environ.c include/environ.h $(CBISON)
	gcc -c -std=c99 $(INCLUDE) -g -o $@ $<

bilquad.o : src/bilquad.c src/util.c include/bilquad.h include/environ.h $(CBISON) 
	gcc -c -std=c99 $(INCLUDE) -g -o $@ $<

tradpp2c3a.o : src/tradpp2c3a.c include/bilquad.h include/util.h include/environ.h $(CBISON)
	gcc -c -std=c99 $(INCLUDE) -g -o $@ $<
	
interppC3A.c: src/interppC3A.l include/environ.h include/bilquad.h include/util.h
	flex -o $@ $< 
	
interppC3A: interppC3A.c bilquad.o environ.o util.o
	$(CC) -std=c99 -Wall -o $@ $^ -lfl

clean:
	rm -f $(CBISON) $(CFLEX) $(EXE) $(HBISON) $(ODEP)
