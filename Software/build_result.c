#include "build_result.h"
#include <stdlib.h>

SDL_Surface *load_surface(char *path)
{
    SDL_Surface *temp = IMG_Load(path);
    SDL_Surface *res = SDL_ConvertSurfaceFormat(temp,SDL_PIXELFORMAT_RGB888,0);

    SDL_FreeSurface(temp);

    return res;
}


SDL_Surface **load_numbers()
{
    SDL_Surface **numbers= malloc(9*sizeof(void *));
    char paths[][40] = {
        "../Image/training/training_1.png",
        "../Image/training/training_2.png",
        "../Image/training/training_3.png",
        "../Image/training/training_4.png",
        "../Image/training/training_5.png",
        "../Image/training/training_6.png",
        "../Image/training/training_7.png",
        "../Image/training/training_8.png",
        "../Image/training/training_9.png",
    };

    for(size_t i = 0; i < 9; i++)
    {
        numbers[i] = load_surface(paths[i]);
    }

    return numbers;
}


void put_surface(SDL_Surface **numbers, SDL_Surface *surface, int  *x, int *y, char n)
{
    SDL_Surface *number = numbers[n-1];

    Uint32* pixelsS = surface->pixels;
    Uint32* pixelsD = number->pixels;
    
    size_t t = 0;
    for(int j = *y;j < number->h + *y; j++)
    {
        for(int i = *x;i < number->w + *x; i++)
        {
            pixelsS[j * (surface->w) + i] = pixelsD[t];
            t++;
        }
    }

    *x += number->w;
}


void free_numbers(SDL_Surface **numbers)
{
    for(size_t x = 0; x < 9; x++)
    {
        SDL_FreeSurface(numbers[x]);
    }
    free(numbers);
}


SDL_Surface *build_result(char **grid)
{
    SDL_Surface *res = load_surface("grid.png");
    SDL_Surface **numbers = load_numbers();
    int i,j;
    j=12;
    for(size_t y = 0; y < 9; y++)
    {
        if(y == 4 || y == 7)
        {
            j += 7;
        }
        i = 12;
        for(size_t x = 0; x < 9; x++)
        {
            if(x == 4 ||x == 7)
            {
                i += 7;
            } 
            put_surface(numbers,res,&i,&j,grid[y][x]);
            i +=8; 
        }
        j += 8 +numbers[0]->h;
    }

    free_numbers(numbers);
    return res;
}
