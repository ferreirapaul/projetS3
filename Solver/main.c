#include "solver.h"

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        errx(EXIT_FAILURE, "Usage : ./solver name");
    }

    char **grid = get_grid(argv[1]);
    
    solve(grid);

    FILE *f;
    int x;
    int y;
    
    f = fopen(strcat(argv[1],".result"),"w");
    x = 0;
    y = 0;
    while(y < 9)
    {
        x = 0;
        while(x< 9)
        {
            if(x == 3 || x == 6)
            {
                fprintf(f," ");
            }
            fprintf(f,"%i",grid[y][x]);
            x++;
        }
        fprintf(f,"\n");
        if(y == 2 || y == 5)
        {
            fprintf(f,"\n");
        }
        y++;
    }
    
    fclose(f);
}
