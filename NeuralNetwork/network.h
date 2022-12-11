#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#define SIZE 784

struct Neuron
{
    double *weights;
    double delta;
    double res;
};

struct Network
{
    struct Neuron *networkHidden;
    struct Neuron *networkOut;
    int isInit;
};

void createInput(char input[9][SIZE], SDL_Surface **training);
void free_neuron(struct Neuron *net);
void printChar(char input[9][SIZE]);
void save_weights(struct Neuron *networkHidden, struct Neuron *networkOut);
char network(char *input, struct Neuron *networkHidden, 
        struct Neuron *networkOut, int isInit);
void load_weights(char *path, struct Neuron *networkHidden, 
        struct Neuron *networkOut);
void save_weights(struct Neuron *networkHidden, struct Neuron *networkOut);



