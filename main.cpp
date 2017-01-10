#include <iostream>
#include "NeuralNetwork.h"

using namespace std;

int main( ) {
    NeuralNetwork nn("parseData\\inputExample");
    nn.run();
    return 0;
}