#pragma once
#include <stdlib.h>
#include <stdio.h>
#define SIZE 784

struct Neuron
{
    double *weights;
    double delta;
    double res;
};



