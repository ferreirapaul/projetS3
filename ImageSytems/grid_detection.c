#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <math.h>


// Updates the display.
//
// renderer: Renderer to draw on.
// texture: Texture that contains the image.
void draw(SDL_Renderer* renderer, SDL_Texture* texture)
{
    SDL_RenderCopy(renderer,texture,NULL,NULL);
    SDL_RenderPresent(renderer);
}

// Event loop that calls the relevant event handler.
//
// renderer: Renderer to draw on.
// colored: Texture that contains the colored image.
// grayscale: Texture that contains the grayscale image.
void event_loop(SDL_Renderer* renderer, SDL_Texture* colored, 
        SDL_Texture* filtered, SDL_Texture* hough)
{
    SDL_Event event;
    SDL_Texture* t = colored;

    while (1)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            // If the "quit" button is pushed, ends the event loop.
            case SDL_QUIT:
                return;

            // If the window is resized, updates and redraws the diagonals.
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    t = filtered;
                    t = hough;
                    draw(renderer, t);
                }
                break;
        }
    }
}

// Loads an image in a surface.
// The format of the surface is SDL_PIXELFORMAT_RGB888.
//
// path: Path of the image.
SDL_Surface* load_image(const char* path)
{
    SDL_Surface* temp = IMG_Load(path);
    if (temp == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Surface* res = SDL_ConvertSurfaceFormat(temp,SDL_PIXELFORMAT_RGB888,0);
    if (res== NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_FreeSurface(temp);
    return res;
}

// Converts a colored pixel into grayscale.
//
// pixel_color: Color of the pixel to convert in the RGB format.
// format: Format of the pixel used by the surface.
Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r,g,b;
    SDL_GetRGB(pixel_color,format,&r,&g,&b);
    Uint8 a = 0.3*r+0.59*g+0.11*b;
    return SDL_MapRGB(format,a,a,a);
}

void filtered(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    int len = surface->w * surface->h;
    SDL_PixelFormat* format = surface->format;
    SDL_LockSurface(surface);
    int lmin = 255;
    int lmax = 0;
    Uint32 r;
    Uint32 g;
    Uint32 b;
    for(int i = 0; i < len; i++)
    {
        SDL_Color rgb;
        SDL_GetRGB(pixels[i], format, &rgb.r, &rgb.g, &rgb.b);
        int l = (rgb.r + rgb.g + rgb.b) / 3;
        int lp = 255 * (l - lmin) / (lmax - lmin);
        if (l != 0)
        {
            r = rgb.r * lp / l;
            g = rgb.g * lp / l;
            b = rgb.b * lp / l;
        }
        else
        {
            r = 255;
            g = 255;
            b = 255;
        }
        pixels[i] = SDL_MapRGB(format, r, g, b);

        pixels[i] = pixel_to_grayscale(pixels[i],format);
    }

    SDL_UnlockSurface(surface);
}

int max(int a, int b)
{
    int max = 0;
    if (a > b)
    {
        max = a;
    }
    else
    {
        max = b;
    }
    return max;
}
 
void hough(SDL_Surface *surface)
{
    
    Uint32* pixels = surface->pixels;
    unsigned int width = surface->w;
    unsigned int height = surface->h;
    
    unsigned int rho, theta;
    rho = sqrt(width * width + height * height);
    unsigned int p;
    theta = 90;

    //initialise accumulator array
    unsigned int acc[rho][theta];
    
    for (size_t i = 0; i < rho; i++)
    {
        for (size_t j = 0; j < theta; j++)
            acc[i][j] = 0;
    }
    printf("%u\n", height);


    Uint32 pixel;
    Uint8 r, g, b;

    //go through each pixels
    for (size_t x = 0; x < width; x++)
    {
        for (size_t y = 0; y < height; y++)
        {
            //printf("testing pixel #%zu\n", x+y*width);
            pixel = pixels[x+y*width];
            SDL_GetRGB(pixel, surface->format, &r, &g, &b);

            

            //if white
            if (r+g+b >= 400)
            {
                
                unsigned int t = 0;
                p = x*cos(t*M_PI/180) + y*sin(t*M_PI/180);
                //printf("testing pixel #%zu to acc value %zu,%i\n",x+y*width, p, t);
                acc[p][t]++;
                t = 90;
                p = x*cos(t*M_PI/180) + y*sin(t*M_PI/180);
                //printf("testing pixel #%zu to acc value %zu,%i\n",x+y*width, p, t);
                acc[p][t]++;
            }
        }
    }
    
    unsigned int intersect[width*height];
    unsigned int vertical[rho/9];
    unsigned int horizontal[rho/9];

    for(p = 0; p < rho; p += 9){
        unsigned int moyenne = 0;
        unsigned int ecart = 0;
        
        for(size_t i = p; i< p+9; i++){
            moyenne += acc[i][0];
        }
        moyenne /= 9;

        for(size_t i=0; i<9; i++){
            ecart += (acc[p+i][0]+moyenne)*(acc[p+i][0]+moyenne);
        }
        ecart /= 9;
        ecart = sqrt(ecart);
        if(moyenne > 90 && ecart < 50){
            horizontal[p/9] = 1;
        }

        moyenne = 0;
        ecart = 0;
        
        for(size_t i = p; i< p+9; i++){
            moyenne += acc[i][90];
        }
        moyenne /= 9;

        for(size_t i=0; i<9; i++){
            ecart += (acc[p+i][0]+moyenne)*(acc[p+i][0]+moyenne);
        }
        ecart /= 9;
        ecart = sqrt(ecart);
        if(moyenne > 90 && ecart < 50){
            vertical[p/9] = 1;
        }
    }

    for(size_t i = 0; i < rho/9; i++){
        if(vertical[i] == 1){
            for(size_t j = 0; j < rho/9; j++){
                if (horizontal[j] == 1){
                    intersect[i*9+j*9*width] = 255;
                }
            }
        }
    }

    SDL_PixelFormat* format = surface->format;
    SDL_LockSurface(surface);
    Uint32 nr;
    Uint32 ng;
    Uint32 nb;
    for(int x = 0; x < width; x++ )
    {
        for(int y = 0; y < height; y++)
        {
            //SDL_Color rgb;
            //SDL_GetRGB(pixels[x + y * width], format, &rgb.r, &rgb.g, &rgb.b);

            nr = intersect[x + y * width];
            ng = intersect[x + y * width];
            nb = intersect[x + y * width];

            if (nr == 255)
            {
                /*pixels[(x-2) + (y-2) * width] = SDL_MapRGB(format, nr, ng, nb);
                pixels[(x-2) + (y-1) * width] = SDL_MapRGB(format, nr, ng, nb);
                pixels[(x-2) + y * width] = SDL_MapRGB(format, nr, ng, nb);
                pixels[(x-2) + (y+1) * width] = SDL_MapRGB(format, nr, ng, nb);
                pixels[(x-2) + (y+2) * width] = SDL_MapRGB(format, nr, ng, nb);
                pixels[(x-1) + (y-2) * width] = SDL_MapRGB(format, nr, ng, nb);
                pixels[(x-1) + (y-1) * width] = SDL_MapRGB(format, nr, ng, nb);
                pixels[(x-1) + y * width] = SDL_MapRGB(format, nr, ng, nb);
                pixels[(x-1) + (y+1) * width] = SDL_MapRGB(format, nr, ng, nb);
                pixels[(x-1) + (y+2) * width] = SDL_MapRGB(format, nr, ng, nb);
                pixels[x + (y-2) * width] = SDL_MapRGB(format, nr, ng, nb);
                pixels[x + (y-1) * width] = SDL_MapRGB(format, nr, ng, nb);
                pixels[x + y * width] = SDL_MapRGB(format, nr, ng, nb);
                pixels[x + (y+1) * width] = SDL_MapRGB(format, nr, ng, nb);
                pixels[x + (y+2) * width] = SDL_MapRGB(format, nr, ng, nb);
                pixels[(x+1) + (y-2) * width] = SDL_MapRGB(format, nr, ng, nb);
                pixels[(x+1) + (y-1) * width] = SDL_MapRGB(format, nr, ng, nb);
                pixels[(x+1) + y * width] = SDL_MapRGB(format, nr, ng, nb);
                pixels[(x+1) + (y+1) * width] = SDL_MapRGB(format, nr, ng, nb);
                pixels[(x+1) + (y+2) * width] = SDL_MapRGB(format, nr, ng, nb);
                pixels[(x+2) + (y-2) * width] = SDL_MapRGB(format, nr, ng, nb);
                pixels[(x+2) + (y-1) * width] = SDL_MapRGB(format, nr, ng, nb);
                pixels[(x+2) + y * width] = SDL_MapRGB(format, nr, ng, nb);
                pixels[(x+2) + (y+1) * width] = SDL_MapRGB(format, nr, ng, nb);
                pixels[(x+2) + (y+2) * width] = SDL_MapRGB(format, nr, ng, nb);*/
                
            }
            else
            {
                pixels[x + y * width] = SDL_MapRGB(format, nr, ng, nb);
            }

        }
    }

    SDL_UnlockSurface(surface);
} 

int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

    // TODO:
    // - Initialize the SDL.
    // - Create a window.
    // - Create a renderer.
    // - Create a surface from the colored image.
    // - Resize the window according to the size of the image.
    // - Create a texture from the colored surface.
    // - Convert the surface into grayscale.
    // - Create a new texture from the grayscale surface.
    // - Free the surface.
    // - Dispatch the events.
    // - Destroy the objects.
    
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Window* window = SDL_CreateWindow("Image display",0,0,100,100, 
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if(window == NULL)
    {
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 
            SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Surface* surface = load_image(argv[1]);

    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    int w = surface->w;
    int h = surface->h;
    SDL_SetWindowSize(window,w,h);
    
    SDL_Texture* textureColored = SDL_CreateTextureFromSurface(renderer, 
            surface);
    if (textureColored == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    filtered(surface);

    SDL_Texture* textureGray = SDL_CreateTextureFromSurface(renderer, surface);
    if (textureGray == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    hough(surface);

    SDL_Texture* textureHough = SDL_CreateTextureFromSurface(renderer, surface);
    if (textureGray == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_FreeSurface(surface);



    event_loop(renderer,textureColored, textureGray, textureHough);



    SDL_DestroyTexture(textureColored);
    SDL_DestroyTexture(textureGray);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();


    return EXIT_SUCCESS;
}



