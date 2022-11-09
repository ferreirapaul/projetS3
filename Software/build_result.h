#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>



SDL_Surface *load_surface(char *path);
SDL_Surface **load_numbers();
void put_surface(SDL_Surface **numbers, SDL_Surface *surface, int* x, int* y, char n);
void free_numbers(SDL_Surface **numbers);
SDL_Surface *build_result(char **grid);
