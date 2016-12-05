//
// Created by Kejsty, Katarina Kejstova on 3.11.16.
//

#ifndef PV021_PROJECT_NEURALNETWORK_H
#define PV021_PROJECT_NEURALNETWORK_H

#include <memory>
#include "InputLayer.h"
#include "HiddenLayer.h"
#include "OutputLayer.h"

class NeuralNetwork {
    std::vector<std::unique_ptr<Layer>> layers;
public:
    const std::vector<std::unique_ptr<Layer>> &getLayers( ) const {
            return layers;
    }

   Layer* getLayer(int layer) {
            return layers[layer].get();
    }

    NeuralNetwork(const std::string &inputPath) {
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

    void run();
};


#endif //PV021_PROJECT_NEURALNETWORK_H
