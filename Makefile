BUILD_PATH := build/
LIBOBJ_PATH = $(BUILD_PATH)lib/

COMPILER_FLAGS := -Wall -ggdb
LINKER_FLAGS := -lSDL2

MAINOBJ := $(BUILD_PATH)main.o
LIBOBJS := $(LIBOBJ_PATH)dynlist.o $(LIBOBJ_PATH)magic.o $(LIBOBJ_PATH)sprite.o $(LIBOBJ_PATH)control.o $(LIBOBJ_PATH)isomap.o

.PHONY: all clean distclean

game: $(MAINOBJ) $(LIBOBJS) $(BUILD_PATH)game.o
	gcc $(COMPILER_FLAGS) $(LINKER_FLAGS) $(wildcard $(BUILD_PATH)*.o) $(LIBOBJS) -o $@.o

$(MAINOBJ): main.cpp
	gcc $(COMPILER_FLAGS) $(LINKER_FLAGS) -c $? -o $@

$(BUILD_PATH)game.o: game.cpp
	gcc $(COMPILER_FLAGS) $(LINKER_FLAGS) -c $? -o $@

$(LIBOBJ_PATH)%.o: lib/%.c
	gcc $(COMPILER_FLAGS) $(LINKER_FLAGS) -c $? -o $@

clean:
	$(RM) $(wildcard $(BUILD_PATH)*.o)
	$(RM) $(wildcard $(LIBOBJ_PATH)*.o) 
	$(RM) game
