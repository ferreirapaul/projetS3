#include "XOR.h"

void NeuralNet(int input[][2], int *output,struct Neuron *network, size_t lenIn)
{
    float target;
    for(size_t i = 0; i < 15000; i++)
    {
        for(size_t j = 0; j < lenIn; j++)
        {
            network[0].res = sin(input[j][0]*network[0].weight1 + 
                    input[j][1]*network[0].weight2);
            network[1].res = sin(input[j][0]*network[1].weight1 + 
                    input[j][1]*network[1].weight2);
            network[2].res = sin(network[0].res*network[2].weight1 +
                    network[1].res*network[2].weight2);

            target = output[j] - network[2].res;

            network[0].bias1 = target * network[2].weight1 * cos(input[j][0]
                    * network[0].weight1 + input[j][1] * network[0].weight2) *
                input[j][0];
            network[0].bias2 = target * network[2].weight1 * cos(input[j][0]
                    * network[0].weight1 + input[j][1] * network[0].weight2) *
                input[j][1];

            network[1].bias1 = target * network[2].weight2 * cos(input[j][0]
                    * network[1].weight1 + input[j][1] * network[1].weight2) *
                input[j][0];
            network[1].bias2 = target * network[2].weight2 * cos(input[j][0]
                    * network[1].weight1 + input[j][1] * network[1].weight2) *
                input[j][1];

            network[2].bias1 = target * cos(network[0].res * network[2].weight1
                    + network[1].res * network[2].weight2) * network[0].res;
            network[2].bias2 = target * cos(network[0].res * network[2].weight1
                    + network[1].res * network[2].weight2) * network[1].res;

            for(size_t t = 0; t < 3; t++)
            {
                network[t].weight1 += network[t].bias1;
                network[t].weight2 += network[t].bias2;
            }
        }
    }
}

void transformIn(int input[][2], int *output, size_t len)
{
    for(int i = 0; i < len; i++)
    {
        input[i][0]++;
        input[i][1]++;
        if(output[i] == 0)
        {
            output[i] = -1;
        }
    }
}

float transformOut(float x)
{
    if(x < 0)
    {
        return x+1.0;
    }
    return x;
}

int main()
{
    int input[][2] = 
    {
        {0,0},
        {0,1},
        {1,0},
        {1,1}
    };

    int output[] = {0,1,1,0};

    //srand(22);

    struct Neuron network[] = {
        {(float)rand()/(float)(RAND_MAX),0,(float)rand()/
            (float)(RAND_MAX),0,0},
        {(float)rand()/(float)(RAND_MAX),0,(float)rand()/
            (float)(RAND_MAX),0,0},
        {(float)rand()/(float)(RAND_MAX),0,(float)rand()/
            (float)(RAND_MAX),0,0},
    };

    transformIn(input,output,4);
    
    NeuralNet(input,output,network,4);

    for(size_t i = 0; i < 4; i++)
    {
        network[0].res = sin(input[i][0]*network[0].weight1 + 
                input[i][1]*network[0].weight2);
        network[1].res = sin(input[i][0]*network[1].weight1 + 
                input[i][1]*network[1].weight2);
        network[2].res = sin(network[0].res*network[2].weight1 +
                network[1].res*network[2].weight2);

        printf("Input : {%i,%i}, output : %f\n", input[i][0]-1, input[i][1]-1,
                transformOut(network[2].res)); 
    }
}
