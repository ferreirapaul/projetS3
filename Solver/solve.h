#pragma once 

#include <err.h>

struct Tuple
{
    int x;
    int y;
};

void solve(char* grid);
unsigned int __solve(char* grid, size_t x, size_t y);
unsigned int check(char* grid, size_t x, size_t y);
unsigned int lineCheck(char* list, size_t x, size_t y, size_t j);
