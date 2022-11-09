#pragma once 

#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Tuple
{
    int x;
    int y;
};

void printBoard(char **grid);
void solve(char **grid);
unsigned int __solve(char **grid, size_t x, size_t y);
unsigned int check(char **grid, size_t x, size_t y);
unsigned int lineCheck(char **grid,char* list, size_t x, size_t y, size_t j);
char **get_grid(char* path);
