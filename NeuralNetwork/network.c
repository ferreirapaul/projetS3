#include "network.h"
#include "../Software/build_result.h"
#include "../Software/soft.h"
#include <time.h>


void forwardHidden(char *input,struct Neuron *networkHidden)
{
    for(size_t i = 0; i < 9; i++)
    {
        networkHidden[i].res = 0;
        for(size_t j = 0; j < SIZE;j++)
        {
            networkHidden[i].res += input[j] * networkHidden[i].weights[j];
        }
        networkHidden[i].res = sin(networkHidden[i].res);
        printf("%f\n",networkHidden[i].res);
    }
}

void forwardOut(struct Neuron *networkHidden, struct Neuron *networkOut)
{
    for(size_t i = 0; i < 9; i++)
    {
        networkOut[i].res = 0;
        for(size_t j = 0; j < 9;j++)
        {
            networkOut[i].res += networkHidden[j].res * networkOut[i].weights[j];
        }
        networkOut[i].res = sin(networkOut[i].res);
    }
}

void forward(char *input, struct Neuron *networkHidden, struct Neuron *networkOut)
{
    forwardHidden(input,networkHidden);
    forwardOut(networkHidden, networkOut);
}

void backwardHidden(char *output, char *input, struct Neuron *networkHidden, const struct Neuron *networkOut)
{
    double target;
    for(size_t i = 0; i < 9; i++)
    {
        target = output[i] - networkOut[i].res;
        for(size_t j = 0; j < SIZE; j++)
        {
            networkHidden[i].bias[j] = 0;
            for(size_t t = 0; t < SIZE; t++)
            {
                networkHidden[i].bias[j] += input[t] * networkHidden[i].weights[t];
            }
            networkHidden[i].bias[j] = target * networkOut[i].weights[i] * cos(networkHidden[i].bias[j])*input[j];
        }
    }
}


void backwardOut(char* output, struct Neuron *networkHidden, struct Neuron *networkOut)
{
    double target;
    for(size_t i = 0; i < 9; i++)
    {
        target = output[i] - networkOut[i].res;
        for(size_t t = 0; t < 9; t++)
        {
            networkOut[i].bias[t] = 0;
            for(size_t j = 0; j < 9; j++)
            {
                networkOut[i].bias[t]+=networkHidden[j].res * networkOut[i].weights[j];
            }
            networkOut[i].bias[t] = target * cos(networkOut[i].bias[t]) * networkHidden[t].res;
        }
    }
}


void backward(char *input, char *output, struct Neuron *networkHidden, struct Neuron *networkOut)
{
    backwardHidden(output, input, networkHidden, networkOut);
    backwardOut(output, networkHidden, networkOut);
    for(size_t i = 0; i < 9;i++)
    {
        for(size_t j = 0; j < SIZE; j++)
        {
            networkHidden[i].weights[j] += networkHidden[i].bias[j];
        }
        for(size_t j = 0; j < 9;j++)
        {
            networkOut[i].weights[j] += networkOut[i].bias[j];
        }
    }
}

void learn(const char input[9][SIZE],const char output[9][9],struct Neuron *networkHidden, struct Neuron *networkOut)
{
    for(size_t j = 0; j < 2000;j++)
    {
        printf("%li\n",j);
        for(size_t i = 0; i < 9; i++)
        {
            forward(input[i],networkHidden,networkOut);
            backward(input[i],output[i],networkHidden,networkOut);
        }
    }
}

void init_in(struct Neuron *network)
{
    for(size_t i = 0; i < 9; i++)
    {
        double weights[SIZE];
        double bias[SIZE];
        for(size_t j = 0; j < SIZE; j++)
        {
            weights[j] = ((double) rand())/((double)(RAND_MAX));
            printf("%f\n",weights[j]);
        }
        struct Neuron n = {weights,bias,0};
        network[i] = n;
    }
}


void init_out(struct Neuron *network)
{
    for(size_t i = 0; i < 9; i++)
    {
        double weights[9];
        double bias[9];
        for(size_t j = 0; j < 9; j++)
        {
            weights[j] = ((double) rand())/((double)(RAND_MAX));
        }
        struct Neuron n = {weights,bias,0};
        network[i] = n;
    }
}

void printChar(char list[9][9])
{
    for(size_t i = 0; i < 9; i++)
    {
        for(size_t j = 0; j < 9; j++)
        {
            printf("%i",list[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}


void transformIn(char **output)
{
    char o1[9] = {1,-1,-1,-1,-1,-1,-1,-1,-1};
    char o2[9] = {-1,1,-1,-1,-1,-1,-1,-1,-1};
    char o3[9] = {-1,-1,1,-1,-1,-1,-1,-1,-1};
    char o4[9] = {-1,-1,-1,1,-1,-1,-1,-1,-1};
    char o5[9] = {-1,-1,-1,-1,1,-1,-1,-1,-1};
    char o6[9] = {-1,-1,-1,-1,-1,1,-1,-1,-1};
    char o7[9] = {-1,-1,-1,-1,-1,-1,1,-1,-1};
    char o8[9] = {-1,-1,-1,-1,-1,-1,-1,1,-1};
    char o9[9] = {-1,-1,-1,-1,-1,-1,-1,-1,1};

    output[0] = o1;
    output[1] = o2;
    output[2] = o3;
    output[3] = o4;
    output[4] = o5;
    output[5] = o6;
    output[6] = o7;
    output[7] = o8;
    output[8] = o9;
    printChar(output);
}

size_t max(struct Neuron *net)
{
    size_t res = 0;
    for(size_t i = 1; i < 9; i++)
    {
        printf("%i : %f\n",i,net[i].res);
        if(net[i].res > net[res].res)
        {
            res = i;
        }
    }
    return res;
}

void createInput(char input[9][SIZE], SDL_Surface **training)
{
    SDL_Surface *c;
    for (size_t i = 0; i < 9; i++)
    {   
        /*
        GdkPixbuf *pix;
        int rowstride, n_channels;
        size_t a;
        guchar *pixels,*p;

        pix = load_pix(training[i]);
        //pix = gdk_pixbuf_scale_simple(pix,28,28,GDK_INTERP_BILINEAR);
        rowstride = gdk_pixbuf_get_rowstride(pix);
        n_channels = gdk_pixbuf_get_n_channels(pix);
        pixels = gdk_pixbuf_get_pixels(pix);
        char t[SIZE];
        a=0;
        for(size_t y = 0; y < 28; y++)
        {
            for(size_t x = 0; x < 28; x++)
            {
                p = pixels + y * rowstride + x * n_channels;
                t[a] = 1;
                if(p[0] == 0 && p[1] == 0 && p[2] == 0)
                {
                    t[a] = 2;
                    printf("1");
                }
                else
                {
                    printf("0");
                }
                a++;
            }
            printf("\n");
        }
        printf("\n");
        printf("\n");
        input[i] = t;
        g_object_unref(pix);*/
        c = training[i];
        Uint32 *pixels = c->pixels;
        Uint8 r,g,b;
        SDL_PixelFormat * format= c->format;
        for(size_t j = 0; j < SIZE;j++)
        {
            input[i][j] = 1;
            SDL_GetRGB(pixels[j], format,&r,&g,&b);
            if(r == 0 && b == 0 && g == 0)
            {
                input[i][j] = 2;
            }
        }
    }
}

void network()
{
    struct Neuron networkHidden[9];
    struct Neuron networkOut[9];
    char output[9][9] = {
        {-1,1,1,1,1,1,1,1,1},
        {1,-1,1,1,1,1,1,1,1},
        {1,1,-1,1,1,1,1,1,1},
        {1,1,1,-1,1,1,1,1,1},
        {1,1,1,1,-1,1,1,1,1},
        {1,1,1,1,1,-1,1,1,1},
        {1,1,1,1,1,1,-1,1,1},
        {1,1,1,1,1,1,1,-1,1},
        {1,1,1,1,1,1,1,1,-1},
        
    };
    SDL_Surface **training = load_numbers();
    char input[9][SIZE];
    

    createInput(input,training);
    //transformIn(output);
    init_in(networkHidden);
    init_in(networkOut);
    learn(input,output,networkHidden,networkOut);

    for(size_t i = 0; i < 9; i++)
    {
        forward(input[i],networkHidden,networkOut);
        printf("%li,%li\n",i+1,max(networkOut));
    }

    free_numbers(training);
}


int main()
{
    network();
    return 1;
}
