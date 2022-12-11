CC = gcc

CPPFLAGS = `pkg-config --cflags sdl2 SDL2_image gtk+-3.0` -MMD -D__NO_INLINE__
CFLAGS = -Wall -Wextra -std=c99 -O1 -g -fsanitize=address 
LDLFLAGS = -lm -lpthread
LDLIBS = `pkg-config --libs sdl2 SDL2_image gtk+-3.0` -rdynamic

BUILD := build
EXEC := main

SRC = soft.c main.c build_result.c ../Solver/solver.c ../NeuralNetwork/network.c
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
	rm -rf NeuralNetwork
	rm -f network.weights
	rm -f grid_res.png
	rm -rf $(BUILD)
	rm -f *.glade~
	$(RM) $(EXEC)
