//
// Created by Kejsty, Katarina Kejstova on 3.11.16.
//

#include "NeuralNetwork.h"

void NeuralNetwork::run( ) {

    try {
        InputLayer *inputLayer = dynamic_cast<InputLayer *>(layers[0].get());
        OutputLayer *outputLayer = dynamic_cast<OutputLayer *>(layers[layers.size()-1].get());
        while ( inputLayer->hasValues()) {
            while ( !inputLayer->eval()) {
                for ( size_t i = 1; i < layers.size(); ++i ) {
                    layers[i].get()->eval();
                }
            }
            for ( size_t i = layers.size(); i > 0; --i ) {
                layers[i - 1].get()->backPropagate(inputLayer->getValues());
            }
            for ( size_t i = layers.size(); i > 0; --i ) {
                layers[i - 1].get()->clear();
            }
        }
        std::cout << "Testing started!\n";
        inputLayer->loadData("parseData/inputExample");
        while ( inputLayer->hasValues()) {
            while ( !inputLayer->eval()) {
                for ( size_t i = 1; i < layers.size(); ++i ) {
                    layers[i].get()->eval();
                }
            }

            if(inputLayer->isEndOfSequence()){
                outputLayer->countSumSquaredError();
            }
            for ( size_t i = layers.size(); i > 0; --i ) {
                layers[i - 1].get()->clear();
            }
        }
    }catch (NeuralNetworkException &e) {
        std::cerr << e.what() << std::endl;
    }
}

NeuralNetwork::NeuralNetwork( const std::string &inputPath ) {
    try {
        layers.push_back(std::make_unique<InputLayer>(inputPath)); //0

        std::vector<Layer *> _layer1;
        _layer1.push_back(layers[0].get());
        layers.push_back(std::make_unique<HiddenLayer>(std::move(_layer1))); //1

        std::vector<Layer *> _layer2;
        _layer2.push_back(layers[0].get());
        _layer2.push_back(layers[1].get());
        layers.push_back(std::make_unique<HiddenLayer>(std::move(_layer2))); //2

        std::vector<Layer *> _layer3;
        _layer3.push_back(layers[0].get());
        _layer3.push_back(layers[2].get());
        layers.push_back(std::make_unique<HiddenLayer>(std::move(_layer3))); //3

        std::vector<Layer *> outLayer;
        outLayer.push_back(layers[1].get());
        outLayer.push_back(layers[2].get());
        outLayer.push_back(layers[3].get());
        layers.push_back(std::make_unique<OutputLayer>(std::move(outLayer), layers[0].get())); //4

        //SETTING above
        HiddenLayer *hiddenLayer1 = dynamic_cast<HiddenLayer *>(layers[1].get());
        HiddenLayer *hiddenLayer2 = dynamic_cast<HiddenLayer *>(layers[2].get());
        HiddenLayer *hiddenLayer3 = dynamic_cast<HiddenLayer *>(layers[3].get());

        hiddenLayer3->setAbove(layers[4].get()); // outputL
        hiddenLayer3->setAbove(layers[3].get()); // me

        hiddenLayer2->setAbove(layers[4].get()); // outputL
        hiddenLayer2->setAbove(layers[3].get()); // above Hiiden L
        hiddenLayer2->setAbove(layers[2].get()); // me

        hiddenLayer1->setAbove(layers[4].get()); // outputL
        hiddenLayer1->setAbove(layers[2].get()); // above Hiiden L
        hiddenLayer1->setAbove(layers[1].get()); // me
    }catch (NeuralNetworkException &e) {
        std::cerr << e.what() << std::endl;
    }
}



