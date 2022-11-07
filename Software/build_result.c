#include "build_result.h"

SDL_Surface *load_surface(char *path)
{
    SDL_Surface *temp = IMG_Load(path);
    SDL_Surface *res = SDL_ConvertSurfaceFormat(temp,SDL_PIXELFORMAT_RGB888,0);
    SDL_FreeSurface(temp);

    return res;
}

SDL_Surface **load_numbers()
{
    SDL_Surface *numbers[9];
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

void put_surface(SDL_Surface **numbers, SDL_Surface *surface, size_t x, size_t y, char n)
{
    SDL_Surface *number = numbers[n-1];

    Uint32* pixelsS = surface->pixels;
    Uint32* pixelsD = number->pixels;
    size_t t = 0;
    for(size_t j = y; j < number->h + y; j++)
    {
        for(size_t i = x; i < number->w + x; i++)
        {
            pixelsS[j*(number->w +x) + i] = pixelsD[t];
            t++;
        }
    }
}

void free_numbers(SDL_Surface **numbers)
{
    for(size_t x = 0; x < 9; x++)
    {
        SDL_FreeSurface(numbers[x]);
    }
}

SDL_Surface *build_result(char grid[][9])
{
    SDL_Surface *res = load_surface("../Image/exemple/grid.png");
    SDL_Surface **numbers = load_numbers();
    int i,j;
    i=12;
    j=12;
    for(size_t y = 0; y < 9; y++)
    {
        if(y == 4 || y == 7)
        {
            j += 5;
        }
        i = 12;
        for(size_t x = 0; x < 9; x++)
        {
            if(x == 4 ||x == 7)
            {
                i += 5;
            } 
            i +=6; 
            j +=6; 
            put_surface(numbers,res,i,j,grid[y][x]);
        }
        j += 6;
    }

    free_numbers(numbers);
    return res;
}
