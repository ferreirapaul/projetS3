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

void solve(char grid[9][9]);
unsigned int __solve(char grid[9][9], size_t x, size_t y);
unsigned int check(char grid[9][9], size_t x, size_t y);
unsigned int lineCheck(char grid[9][9],char* list, size_t x, size_t y, size_t j);
char **get_grid(char* path);
