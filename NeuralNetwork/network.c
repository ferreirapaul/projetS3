#include "network.h"
#include "../Software/build_result.h"

void forwardHidden(char *input,struct Neuron *networkHidden)
{
    for(size_t i = 0; i < 9; i++)
    {
        for(size_t j = 0; j < 10000;j++)
        {
            networkHidden[i].res += input[j] * networkHidden[i].weights[j];
        }
        networkHidden[i].res = sin(networkHidden[i].res);
    }
}

void forwardOut(struct Neuron *networkHidden, struct Neuron *networkOut)
{
    for(size_t i = 0; i < 9; i++)
    {
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

void backwardHidden(char *output, char *input, struct Neuron *networkHidden, struct Neuron *networkOut)
{
    double target;
    for(size_t i = 0; i < 9; i++)
    {
        target = output[i] - networkOut[i].res;
        for(size_t j = 0; j < 10000; j++)
        {
            for(size_t t = 0; t < 10000; t++)
            {
                networkHidden[i].bias[j] += input[t] * networkHidden[i].weights[t];
            }
            networkHidden[i].bias[j] = target * networkOut[i].res * cos(networkHidden[i].bias[j])*input[j];
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
            for(size_t j = 0; j < 9; j++)
            {
                networkOut[i].bias[t]+=networkHidden[j].res * networkOut[i].weights[j];
            }
            networkOut[i].bias[t] = target * cos(networkOut[i].bias[t]) * networkHidden[i].res;
        }
    }
}


void backward(char *input, char *output, struct Neuron *networkHidden, struct Neuron *networkOut)
{
    backwardHidden(output, input, networkHidden, networkOut);
    backwardOut(output, networkHidden, networkOut);
    for(size_t i = 0; i < 9;i++)
    {
        for(size_t j = 0; j < 10000; j++)
        {
            networkHidden[i].weights[j] += networkHidden[i].bias[j];
        }
        for(size_t j = 0; j < 9;j++)
        {
            networkOut[i].weights[j] += networkOut[i].bias[j];
        }
    }
}

void learn(char **input, char **output,struct Neuron *networkHidden, struct Neuron *networkOut)
{
    for(size_t j = 0; j < 2000;j++)
    {
        for(size_t i = 0; i < 9; i++)
        {
            forward(input[i],networkHidden,networkOut);
            backward(input[i],output[i],networkHidden,networkOut);
        }
    }
}

void init(struct Neuron *network)
{
    for(size_t i = 0; i < 9; i++)
    {
        double weights[10000];
        double bias[10000];
        for(size_t j = 0; j < 10000; j++)
        {
            weights[j] = (float) rand()/(float)(RAND_MAX);
            bias[j] = (float) rand()/(float)(RAND_MAX);
        }
        struct Neuron n = {weights,bias,0};
        network[i] = n;
    }
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
}

double max(struct Neuron *net)
{
    double res = net[0].res;
    for(size_t i = 1; i < 9; i++)
    {
        if(net[i].res > res)
        {
            res = net[i].res;
        }
    }
    return res;
}

void createInput(char **input, SDL_Surface **training)
{
    Uint32 *p;
    Uint8 r,g,b;
    SDL_PixelFormat* format = training[0]->format;
    for (size_t i = 0; i < 9; i++)
    {
        char t[10000];
        input[i] = t;
        p = training[i]->pixels;
        for(size_t j = 0; j < 10000; j++)
        {
            SDL_GetRGB(p[j],format,&r,&g,&b);
            input[i][j] = 1;
            if(r == 0 && g == 0 && b == 0)
            {
                input[i][j] = 2;
            }
        }
    }
}

void freeChar(char **l)
{
    for(size_t i = 0; i < 9; i++)
    {
        free(l[i]);
    }
    free(l);
}

void network()
{
    struct Neuron networkHidden[10];
    struct Neuron networkOut[10];
    char **output = malloc(9*sizeof(void*));
    SDL_Surface **training = load_numbers();
    char **input = malloc(9*sizeof(void*));

    createInput(input,training);
    transformIn(output);
    init(networkHidden);
    init(networkOut);

    learn(input,output,networkHidden,networkOut);

    for(size_t i = 0; i < 9; i++)
    {
        forward(input[i],networkHidden,networkOut);
        backward(input[i],output[i],networkHidden,networkOut);
        printf("%i,%d\n",i,networkOut[i].res);
    }

    freeChar(output);
    freeChar(input);
    free_numbers(training);
}


int main()
{
    network();
}
