CC = gcc -Wall -g
CINCLUDE = -Iinclude
LDFLAGS = -L$(LIB) -l$(NOM_LIB)
DFLAGS = -DMODE

EXEC = main
NOM_LIB = jeu

#VERSION = $(shell git describe --tags)
VERSION = 0.5.2
ARCHIVE = PulidoCornejo_David-GoL-$(VERSION)
SOURCES = src/ include/ Makefile Doxyfile

#on selectionne tous les .c dans src avec $(wildcard src/*.c) et pour chacun on remplace le .c par .o avec patsubst et le %
OBJECTS = $(patsubst src/%.c, obj/%.o, $(wildcard src/*.c))
OBJECTS := $(filter-out obj/$(EXEC).o,$(OBJECTS))

vpath %.o obj
vpath %.c src
vpath %.h include
vpath %.a lib

BIN = bin/
OBJ = obj/
DOC = doc/
LIB = lib/
SRC = src/

ifeq ($(MODE),TEXTE)
	DFLAGS = -DMODE=TEXTE
	OBJECTS := $(filter-out obj/io_cairo.o,$(OBJECTS))
	RM = rm -f  $(OBJ)/io_cairo.o
else
	DFLAGS = -DMODE=GRAPHIQUE
	CINCLUDE +=  -I/usr/include/cairo
	LDFLAGS += -lcairo -lm -lX11
	OBJECTS := $(filter-out obj/io.o,$(OBJECTS))
	RM = rm -f  $(OBJ)/io.o
endif

#bin/main
$(EXEC) : lib$(NOM_LIB).a
	mkdir -p $(BIN)
	$(CC) -o $(BIN)$@ $(SRC)$(EXEC).c $(LDFLAGS) $(CINCLUDE) $(DFLAGS)

$(OBJ)%.o : %.c %.h
	mkdir -p $(OBJ)
	$(CC) -c $< $(CINCLUDE)
	mv -f *.o $(OBJ)

$(LIB)lib$(NOM_LIB).a : $(OBJECTS)
	mkdir -p $(LIB)
	ar -crv $@ $^
	ranlib $@

$(ARCHIVE).tar.xz : $(SOURCES)
	tar -c --lzma -f $@ $(SOURCES)

Doxyfile :
	doxygen -g

docs : Doxyfile
	rm -rf $(DOC)
	doxygen
	mv html/ $(DOC)

clean :
	rm -f $(OBJ)*.o $(BIN)$(EXEC) $(LIB)*.a

dist : $(ARCHIVE).tar.xz

