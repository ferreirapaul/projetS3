#include "solver.h"
#include <err.h>
#include <stdio.h>

void printBoard(char **grid)
{
    for(size_t y = 0; y < 9; y++)
    {
        for(size_t x = 0; x < 9; x++)
        {
            printf("%i", grid[y][x]);
        }
        printf("\n");
    }
}

unsigned int lineCheck(char **grid, char* list, size_t x, size_t y, size_t j)
{
    unsigned int res = 1;
    int t = 0;
    while(t<j && res)
    {
        if(grid[y][x] == list[t] && grid[y][x] != 0)
        {
            res = 0;
        }
        t++;
    }
    return res; 
}


unsigned int check(char **grid, size_t x, size_t y)
{
    unsigned int res = 1;
    char list[]= {0,0,0,0,0,0,0,0,0,0,0};

    int i = x;  //X axis
    int j = 0;
    
    while(i < 9 && res)
    {
        res = lineCheck(grid,list,i,y,j);        
        list[j] = grid[y][i];
        i++;
        j++;
    }
    
    i = x-1;
    while(i > -1 && res)
    {
        res = lineCheck(grid,list,i,y,j);        
        list[j] = grid[y][i];
        i--;
        j++;
    }
       
    i = 0; //reset list
    while(i < 11)
    {
        list[i] = 0;
        i++;
    }
    j = 0;
    
    
    i = y; //Y axis
    while(i < 9 && res)
    {
        res = lineCheck(grid,list,x,i,j);        
        list[j] = grid[i][x];
        i++;
        j++;
    }
    i = y-1;
    while(i > -1 && res)
    {
        res = lineCheck(grid,list,x,i,j);        
        list[j] = grid[i][x];
        i--;
        j++;
    }

    i = 0; //reset list
    while(i < 11)
    {
        list[i] = 0;
        i++;
    }
    j = 0;

    switch(x/3)
    {
        case 0: 
            x = 1;
            break;
        case 1:
            x = 4;
            break;
        default:
            x = 7;
            break;
    }
    
    switch(y/3)
    {
        case 0: 
            y = 1;
            break;
        case 1:
            y = 4;
            break;
        default:
            y = 7;
            break;
    }



    struct Tuple test[] = {{x+1,y+1},{x-1,y-1},{x,y+1},{x,y-1},{x+1,y},{x-1,y},
                    {x,y},{x-1,y+1},{x+1,y-1}};
    
    i = 0;
    while(i<9 && res)
    {
        res = lineCheck(grid,list,test[i].x,test[i].y,i);
        list[j] = grid[test[i].y][test[i].x];
        j++;
        i++;
    }
    
    return res;
}


unsigned int __solve(char **grid, size_t x, size_t y)
{   //recursive fonction that solve the board return false when no solutions are possible
    if(x == 9)
    {
        if(y == 8)
        {
            return (unsigned int) 1;
        }
        else
        {
            return __solve(grid,0,y+1);
        }
    }

    if(grid[y][x] != 0)
    {
        return __solve(grid, x+1,y);
    }

    unsigned int res = 0;
    unsigned int n = 1;

    while(n < 10 && !res)
    {
        grid[y][x] = n;
        if(check(grid,x,y))
        {
            res = __solve(grid, x+1,y);
        }

        if(!res)
        {
            grid[y][x] = 0;
        }
        n++;
    } 
    return res;
}


char** get_grid(char* path)
{
    FILE* f;
    char **grid = malloc(81);
    char c;

    f = fopen(path, "r");
    if(f == NULL)
    {
        free(grid);
        errx(EXIT_FAILURE, "error in the opening of the file");
    }
    
    int x = 0;
    int y = 0;
    grid[0] = malloc(9);
    while(!feof(f))
    {
        if(x == 9)
        {
            if(y == 9)
            {
                errx(EXIT_FAILURE, "the file format is not respected");
            }
            x = 0;
            y++;
            grid[y] = malloc(9);
        }
        
        c = fgetc(f);
        if(c!= ' ' && c!= '\n')
        {
            if(c=='.')
            {
                grid[y][x] = 0;
            }
            else
            {
                grid[y][x] = c - '0'; 
            }
            x++;
        }
    }

    return grid;
}


void solve(char **grid)
{ 
    unsigned int res = __solve(grid,0,0);
    if(!res)
    {
        errx(EXIT_FAILURE,"board is unsolvable");
    }
}
