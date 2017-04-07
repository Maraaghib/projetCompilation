SRCBISON = ppascal.y
SRCFLEX = $(SRCBISON:.y=.l)
CBISON = $(SRCBISON:.y=bison.c)
HBISON = $(CBISON:.c=.h)
CFLEX = $(SRCFLEX:.l=flex.c)
CDEP = util.c environ.c bilquad.c tradpp2c3a.c
ODEP = $(CDEP:.c=.o)
INCLUDE = -I include/
EXE = ppascal
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

bilquad.o : src/bilquad.c include/bilquad.h include/environ.h $(CBISON)
	gcc -c -std=c99 $(INCLUDE) -g -o $@ $<

tradpp2c3a.o : src/tradpp2c3a.c include/bilquad.h include/util.h include/environ.h $(CBISON)
	gcc -c -std=c99 $(INCLUDE) -g -o $@ $<

clean:
	rm -f $(CBISON) $(CFLEX) $(EXE) $(HBISON) $(ODEP)
