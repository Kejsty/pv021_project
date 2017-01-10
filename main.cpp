#include <iostream>
#include "NeuralNetwork.h"

using namespace std;

int main( ) {
    NeuralNetwork nn("parseData\\input_007_prolonged.txt");
    nn.run();
    return 0;
}