#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int main(int argc, char** argv)
{
    FILE* f;
    char[9][9] grid;
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

    //solve
}
