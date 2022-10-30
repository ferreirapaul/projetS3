# Makefile



CC = gcc

CPPFLAGS =

CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`

LDFLAGS =

LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm



all: display grid_detection



SRC = display.c grid_detection.c

OBJ = ${SRC:.c=.o}

EXE = ${SRC:.c=}



display: display.o

grid_detection: grid_detection.o



.PHONY: clean



clean:
	${RM} ${OBJ}
	${RM} ${EXE}



# END

