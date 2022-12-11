#include "network.h"
#include "../Software/build_result.h"
#include "../Software/soft.h"
#include <time.h>

double relu(double x)
{
    if(x > 0)
    {
        return x;
    }
    return 0;
}

void softmax(struct Neuron *net)
{
    double sum = 0.0;
    for(size_t i = 0; i < 9;i++)
    {
        net[i].res = exp(net[i].res);
        sum += net[i].res;
    }

    for(size_t i = 0; i < 9;i++)
    {
        net[i].res = net[i].res / sum;
    }
}

double relu_deriv(double x)
{
    if(x > 0)
    {
        return 1;
    }
    return 0;
}

void forwardHidden(char *input,struct Neuron *networkHidden)
{
    for(size_t i = 0; i < 9; i++)
    {
        double sum = 0.0;
        for(size_t j = 0; j < SIZE;j++)
        {
            sum += input[j] * networkHidden[i].weights[j];
        }
        networkHidden[i].res = relu(sum);
    }
}

void forwardOut(struct Neuron *networkHidden, struct Neuron *networkOut)
{
    for(size_t i = 0; i < 9; i++)
    {
        double sum = 0.0;
        for(size_t j = 0; j < 9;j++)
        {
            sum += networkHidden[j].res * networkOut[i].weights[j];
        }
        networkOut[i].res = sum;
    }
    softmax(networkOut);
}

void forward(char *input, struct Neuron *networkHidden, struct Neuron *networkOut)
{
    forwardHidden(input,networkHidden);
    forwardOut(networkHidden, networkOut);
}

void backward(char *input, double *output, struct Neuron *networkHidden, struct Neuron *networkOut)
{
    double errorTemp = 0.0;
    for(size_t i = 0; i < 9; i++)
    {
        errorTemp = output[i] - networkOut[i].res;
        networkOut[i].delta = errorTemp * relu_deriv(networkOut[i].res);
    }

    for(size_t i = 0; i < 9; i++)
    {
        errorTemp = 0.0;
        for(size_t j = 0; j < 9; j++)
        {
            errorTemp += networkOut[j].delta * networkOut[j].weights[i];
        }
        networkHidden[i].delta = errorTemp * relu_deriv(networkHidden[i].res);
    }

    for(size_t i = 0; i < 9; i++)
    {
        for(size_t j = 0; j < SIZE; j++)
        {
            networkHidden[i].weights[j] += networkHidden[i].delta * input[j] * 0.01;
        }

        for(size_t j = 0; j < 9; j++)
        {
            networkOut[i].weights[j] += networkHidden[j].res * networkOut[i].delta * 0.01;
        }
    }
}



void init_in(struct Neuron *network)
{
    for(size_t i = 0; i < 9; i++)
    {
        double *weights = malloc(SIZE * sizeof(double));
        for(size_t j = 0; j < SIZE; j++)
        {
            weights[j] = (double)rand()/ RAND_MAX * 2.0 - 1.0;
        }
        struct Neuron n = {weights,0,0};
        network[i] = n;
    }
}


void init_out(struct Neuron *network)
{
    for(size_t i = 0; i < 9; i++)
    {
        double *weights = malloc(9*sizeof(double));
        for(size_t j = 0; j < 9; j++)
        {
            weights[j] = (double)rand()/ RAND_MAX * 2.0 - 1.0; 
        }
        struct Neuron n = {weights,0,0};
        network[i] = n;
    }
}

void printChar(char input[9][SIZE])
{
    for(size_t t = 0; t < 9; t++)
    {
        for(size_t i = 0; i < 9; i++)
        {
            for(size_t j = 0; j < 9; j++)
            {
                printf("%i",input[t][i*9 + j]);
            }
            printf("\n");
        }
        printf("\n\n");
    }
}


char max(struct Neuron *net)
{
    char res = 0;
    size_t j = 0;
    //printf("%f\n",net[0].res);
    for(size_t i = 1; i < 9; i++)
    {
        //printf("%f\n",net[i].res);
        if(net[i].res > net[j].res)
        {
            res =(char) i;
            j = i;
        }
    }
    return res;
}


void createInput(char input[9][SIZE],SDL_Surface **training)
{
    SDL_Surface *c;
    for (size_t i = 0; i < 9; i++)
    {   
        Uint8 r,g,b;
        c = training[i];
        SDL_PixelFormat *format= c->format;
        Uint32 *pixels = c->pixels;
        for(size_t j = 0; j < SIZE;j++)
        {
            input[i][j] = 0;
            SDL_GetRGB(pixels[j], format,&r,&g,&b);
            if(r == 0 && b == 0 && g == 0)
            {
                input[i][j] = 2;
            }
        }
    }
}

void free_neuron(struct Neuron *net)
{
    for(size_t i = 0; i < 9; i++)
    {
        free(net[i].weights);
    }
}


void learn(struct Neuron *networkHidden, struct Neuron *networkOut)
{   
    srand(time(NULL));
    init_in(networkHidden);
    init_in(networkOut);
    double output[9][9] = {
        {1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
        {0.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
        {0.0,0.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0},
        {0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,0.0},
        {0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0},
        {0.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0},
        {0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0},
        {0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0},
        {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0},
    };
    
    SDL_Surface **training = load_training();
    char input[9][SIZE]; 
    createInput(input,training);

    for(size_t j = 0; j < 20000;j++)
    {
        //printf("%li\n",j);
        for(size_t i = 0; i < 9; i++)
        {
            forward(input[i],networkHidden,networkOut);
            backward(input[i],output[i],networkHidden,networkOut);
        }
    }

    free_numbers(training);
}


void save_weights(struct Neuron *networkHidden, struct Neuron *networkOut)
{
    learn(networkHidden,networkOut);

    FILE *f = fopen("network.weights","w");
    //fprintf(f,"######NEURAL NETWORK######\n");
    for(size_t i = 0; i < 9; i++)
    {
        //fprintf(f,"Neurons : %li\nHidden:\n",i);
        for(size_t j = 0; j < SIZE;j++)
        {
            fprintf(f,"%f ",networkHidden[i].weights[j]);
        }
       // fprintf(f,"\nOut:\n");
        for(size_t j = 0; j < 9;j++)
        {
            fprintf(f,"%f ",networkOut[i].weights[j]);
        }
        //fprintf(f,"\n");
    }
    fclose(f);
}

void load_weights(char *path, struct Neuron *networkHidden, struct Neuron *networkOut)
{
    init_in(networkHidden);
    init_in(networkOut);
    FILE *f = fopen(path,"r");

    if(f == NULL)
    {
        errx(EXIT_FAILURE,"error in the opening of the file");
    }

    size_t i = 0;
    size_t j = 0;
    int isHidden = 1;
    double x;
    while(i < 9 && fscanf(f, "%lf ",&x))
    {
        if(isHidden && j >= SIZE)
        {
            j = 0;
            isHidden = 0;
        }

        if(isHidden)
        {
            networkHidden[i].weights[j] = x;
        }
        else
        {
            networkOut[i].weights[j] = x;
        }
        j++;
        
        if(!isHidden && j >= 9)
        {
            j = 0;
            i++;
            isHidden = 1;
        }

    }
}

char network(char *input, struct Neuron *networkHidden, struct Neuron *networkOut, int isInit)
{
    if(!isInit)
    {
        learn(networkHidden, networkOut);
    }

    forward(input,networkHidden,networkOut);
    return max(networkOut)+1;

}
