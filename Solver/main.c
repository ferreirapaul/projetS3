#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include "solver.h"

int main(int argc, char** argv)
{
    FILE* f;
    char grid[9][9];
    char c;

    if(argc != 2)
    {
        errx(EXIT_FAILURE, "Usage : ./solver name");
    }

    f = fopen(argv[1], "r");
    if(f == NULL)
    {
        errx(EXIT_FAILURE, "error in the opening of the file");
    }
    
    int x = 0;
    int y = 0;
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

    fclose(f);

    solve(grid);

    f = fopen(strcat(argv[1],".result"),"w");
    x = 0;
    y = 0;
    while(y < 9)
    {
        x = 0;
        while(x< 9)
        {
            if(x == 2 || x == 5)
            {
                fprintf(f," ");
            }
            fprintf(f,"%i",grid[y][x]);
            x++;
        }
        fprintf(f,"\n");
        y++;
    }
    
    fclose(f);
}
