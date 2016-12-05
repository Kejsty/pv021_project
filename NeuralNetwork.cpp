//
// Created by Kejsty, Katarina Kejstova on 3.11.16.
//

#include "NeuralNetwork.h"

void NeuralNetwork::run( ) {

    try {
        InputLayer *inputLayer = dynamic_cast<InputLayer *>(layers[0].get());
        while ( inputLayer->hasValues()) {
            while ( !inputLayer->eval()) {
                for ( size_t i = 1; i < layers.size(); ++i ) {
                    layers[i].get()->eval();
                }
            }
            for ( size_t i = layers.size(); i > 0; --i ) {
                layers[i - 1].get()->backPropagate(inputLayer->getValues());
            }
            inputLayer->reset();
        }
    }catch (NeuralNetworkException &e) {
        std::cerr << e.what() << std::endl;
    }
}

