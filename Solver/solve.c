#include "solve.h"


unsigned int lineCheck(char* list, size_t x, size_t y, size_t j)
{
    unsigned int res;
    int t = 0;
    while(t<j && res)
    {
        if(grid[y][x] == list[t])
        {
            res = 0;
        }
        t++;
    }
    return res; 
}


unsigned int check(char* grid, size_t x, size_t y)
{
    unsigned int res = 1;
    char[9] list = {0};

    int i = x;  //X axis
    int j = 0;
    while(i < 9 && res)
    {
        res = lineCheck(list,i,y,j);        
        list[j] = grid[y][i];
        i++;
        j++;
    }

    i = x;
    while(i > -1 && res)
    {
        res = lineCheck(list,i,y,j);        
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
        res = lineCheck(list,x,i,j);        
        list[j] = grid[i][x];
        i++;
        j++;
    }
    
    i = y;
    while(i > -1 && res)
    {
        res = lineCheck(list,i,y,j);        
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

    Tuple[] test = {{x+1,y+1},{x-1,y-1},{x,y+1},{x,y-1},{x+1,y},{x-1,y},
                    {x,y},{x-1,y+1},{x+1,y-1}};

    i = 0;
    while(i<9 && res)
    {
        res = lineCheck(list,test[i].x,text[i].y,i);
        list[j] = grid[test[i].y][test[i].x];
        i++;
    }

    return res;
}


unsigned int __solve(char* grid, size_t x, size_t y)
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

    unsigned int res = 1;
    unsigned int n = 1;

    while(n < 10 && !res)
    {
        grid[y][x] = n;
        if(check)
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


void solve(char* grid)
{
    unsigned int res = __solve(grid,0,0);
    if(!res)
    {
        errx(EXIT_FAILURE,"board is unsolvable");
    }
}
