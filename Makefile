COMPILER_FLAGS := -Wall -ggdb
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

OBJS := \
	$(MAINOBJ) \
	$(LIBOBJS) \

.PHONY: all clean distclean
.SUFFIXES: .o .c .cpp

all: rpggame

rpggame: $(OBJS)
	$(CXX) -o $@.o $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS)

c.o:
	$(CC) -c $< -o $@ $(COMPILER_FLAGS)

.cpp.o:
	$(CXX) -c $< -o $@ $(COMPILER_FLAGS)

clean:
	$(RM) main.o
	$(RM) game.o
	$(RM) $(wildcard lib/*.o) 
	$(RM) rpggame.o
