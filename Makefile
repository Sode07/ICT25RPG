BUILD_PATH := build/
LIBOBJ_PATH = $(BUILD_PATH)lib/

COMPILER_FLAGS := -Wall -ggdb
LINKER_FLAGS := -l SDL2

MAINOBJ := $(BUILD_PATH)main.o

.PHONY: all clean distclean

game: $(MAINOBJ)
	$(CXX) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(wildcard $(BUILD_PATH)*.o) -o $@.o

$(MAINOBJ): main.cpp
	$(CXX) $(COMPILER_FLAGS) $(LINKER_FLAGS) -c $? -o $@

clean:
	$(RM) $(wildcard $(BUILD_PATH)*.o)
	$(RM) game
