#include "solver.h"
#include <err.h>

unsigned int lineCheck(char grid[9][9], char* list, size_t x, size_t y, size_t j)
{
    unsigned int res = 1;
    int t = 0;
    while(t<j && res)
    {
        /*
        printf("list : grid, x: %i, y: %i, %i\n",x,y,grid[y][x]);
        printf("list : l: t: %i, %i\n",t,list[t]);*/
        if(grid[y][x] == list[t] && grid[y][x] != 0)
        {
            /*
            printf("list : grid, x: %i, y: %i, %i\n",x,y,grid[x][y]);
            printf("list : l: t: %i, %i\n",t,list[t]);*/
            res = 0;
        }
        t++;
    }
    return res; 
}


unsigned int check(char grid[9][9], size_t x, size_t y)
{
    printf("c: x : %i, y : %i, %i\n",x,y,grid[x][y]);
    unsigned int res = 1;
    char list[9]= {0};

    int i = x;  //X axis
    int j = 0;
    while(i < 9 && res)
    {
        res = lineCheck(grid,list,i,y,j);        
        list[j] = grid[y][i];
        i++;
        j++;
    }
    
    i = x;
    while(i > -1 && res)
    {
        res = lineCheck(grid,list,i,y,j);        
        list[j] = grid[y][i];
        i--;
        j++;
    }
    
    i = 0; //reset list
    while(i < 9)
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
    
    i = y;
    while(i > -1 && res)
    {
        res = lineCheck(grid,list,i,y,j);        
        list[j] = grid[y][i];
        i--;
        j++;
    }
    
    i = 0; //reset list
    while(i < 9)
    {
        list[i] = 0;
        i++;
    }

    struct Tuple test[] = {{x+1,y+1},{x-1,y-1},{x,y+1},{x,y-1},{x+1,y},{x-1,y},
                    {x,y},{x-1,y+1},{x+1,y-1}};

    i = 0;
    while(i<9 && res)
    {
        res = lineCheck(grid,list,test[i].x,test[i].y,i);
        list[j] = grid[test[i].y][test[i].x];
        i++;
    }
    
    return res;
}


unsigned int __solve(char grid[9][9], size_t x, size_t y)
{   //recursive fonction that solve the board return false when no solutions are possible
    if(x == 9)
    {
        if(y == 9)
        {
            return 1;
        }
        return __solve(grid,0,y+1);
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
            printf("b");
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


void solve(char grid[9][9])
{
    unsigned int res = __solve(grid,0,0);
    if(!res)
    {
        errx(EXIT_FAILURE,"board is unsolvable");
    }
}
