//
// Created by Kejsty, Katarina Kejstova on 3.11.16.
//

#ifndef PV021_PROJECT_NEURALNETWORK_H
#define PV021_PROJECT_NEURALNETWORK_H

#include <memory>
#include "InputLayer.h"
#include "HiddenLayer.h"
#include "OutputLayer.h"
#include "ImageMaker.h"

class NeuralNetwork {
    std::vector<std::unique_ptr<Layer>> layers;
    ImageMaker imageMaker;
public:
    const std::vector<std::unique_ptr<Layer>> &getLayers( ) const {
            return layers;
    }

   Layer* getLayer(int layer) {
            return layers[layer].get();
    }

    NeuralNetwork(const std::string &inputPath) ;

    void run();
    void renderProbs(std::vector<double> location);
};


#endif //PV021_PROJECT_NEURALNETWORK_H
