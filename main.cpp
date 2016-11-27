#include <iostream>
#include "NeuralNetwork.h"

using namespace std;

int main( ) {
    NeuralNetwork nn("parseData/output");
    nn.run();
    return 0;
}