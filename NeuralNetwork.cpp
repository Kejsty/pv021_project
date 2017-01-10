//
// Created by Kejsty, Katarina Kejstova on 3.11.16.
//

#include "NeuralNetwork.h"

void NeuralNetwork::renderProbs(std::vector<double> curr){
    std::vector<std::vector<double>> points;
    OutputLayer *outputLayer = dynamic_cast<OutputLayer *>(layers[layers.size()-1].get());
    double prob;
    std::vector<double> point (3);
    for(double x = curr[0] - 0.1; x <= curr[0] + 0.1; x+=0.01)
        for(double y = curr[0] - 0.1; y <= curr[0] + 0.1; y+=0.01) {
            point[0] = x;
            point[1] = y;
            point[2] = outputLayer->getProbOfInput(point);
            points.push_back(point);
        }
    imageMaker.renderProbs("", points);
}

void NeuralNetwork::run( ) {

    try {
        InputLayer *inputLayer = dynamic_cast<InputLayer *>(layers[0].get());
        OutputLayer *outputLayer = dynamic_cast<OutputLayer *>(layers[layers.size()-1].get());
        std::vector<double> curr;
        std::vector<double> last;
        int counter = 0;
        int sequence_counter = 0;
        while ( inputLayer->hasValues()) {
            counter = 0;
            while ( !inputLayer->eval()) {
                //update ImageMaker
                last = curr;
                curr = inputLayer->getValues();
                imageMaker.addLocation(curr);

                if(counter > 0 and sequence_counter > 0 ) { //start after 1st sequence
                    renderProbs(curr); //render prob distribution for the next point
                }

                for ( size_t i = 1; i < layers.size(); ++i ) {
                    layers[i].get()->eval();
                }

                counter++;
            }
            if(inputLayer->isEndOfSequence()) {
#if PRINT
                std::cout << "end of " << sequence_counter << ". sequence" << std::endl;
#endif
                outputLayer->countSumSquaredError();
                //imageMaker.renderInput("");
                imageMaker.restart();
                sequence_counter++;
            }
            for ( size_t i = layers.size(); i > 0; --i ) {
                layers[i - 1].get()->backPropagate(inputLayer->getValues());
            }
            for ( size_t i = layers.size(); i > 0; --i ) {
                layers[i - 1].get()->clear();
            }
        }
        std::cout << "Testing started!\n";
        inputLayer->loadData("C:\\Users\\xbendik\\ClionProjects\\pv021_project\\parseData\\output");
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



