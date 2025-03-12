COMPILER_FLAGS := -Wall -ggdb -D _DEBUG
LINKER_FLAGS := -lSDL2 -lstdc++

MAINOBJ := \
	main.o \
	game.o \

LIBOBJS := \
	lib/dynlist.o \
	lib/magic.o   \
	lib/sprite.o  \
	lib/control.o \
	lib/isomap.o  \
	lib/console.o \
	lib/juustoloader.o \

OBJS := \
	$(MAINOBJ) \
	$(LIBOBJS) \

.PHONY: all clean distclean
.SUFFIXES: .c .o .cpp

all: rpggame

rpggame: $(OBJS)
	$(CXX) -o $@ $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS)

.c.o:
	$(CC) $(COMPILER_FLAGS) -std=c99 -c $< -o $@ 

.cpp.o:
	$(CXX) $(COMPILER_FLAGS) -c $< -o $@ 

clean:
	$(RM) main.o
	$(RM) game.o
	$(RM) $(wildcard lib/*.o) 
	$(RM) rpggame
