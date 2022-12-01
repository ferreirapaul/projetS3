CC = gcc

CPPFLAGS = `pkg-config --cflags sdl2 SDL2_image` -MMD -D__NO_INLINE__
CFLAGS = -Wall -Wextra -std=c99 -O1 -g -fsanitize=address 
LDLFLAGS = -lm -lpthread
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -rdynamic

BUILD := build Software
EXEC := main

SRC = network.c ../Software/build_result.c
OBJ = $(SRC:%.c=$(BUILD)/%.o)
DEP = $(SRC:%.c=$(BUILD)/%.d)

all: init UI clear

clear:
	find . -type d -empty -delete

init: 
	$(shell mkdir -p $(BUILD))
	$(shell mkdir -p $(SRC:%=$(BUILD)/%))

UI: $(OBJ)
	gcc -o $(EXEC) $(CFLAGS) $^ $(LDLFLAGS) $(LDLIBS)

$(BUILD)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDLFLAGS) $(CPPFLAGS) $(LDLIBS)

# Format all files
format:
	find -name "*.[ch]" -exec clang-format --verbose -i {} \;

clean:
	rm -rf Solver
	rm -r *.o
	rm -rf $(BUILD)
	rm -f *.glade~
	$(RM) $(EXEC)
