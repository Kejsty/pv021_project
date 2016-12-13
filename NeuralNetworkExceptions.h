//
// Created by Katarína Kejstová on 02/12/16.
//

#ifndef PV021_PROJECT_NEURALNETWORKEXCEPTIONS_H
#define PV021_PROJECT_NEURALNETWORKEXCEPTIONS_H

#include <string>
#include <exception>

struct NeuralNetworkException : std::runtime_error{
    NeuralNetworkException(const std::string &message)  : std::runtime_error(message) {}

};

struct InvalidInputFileException : NeuralNetworkException {
    InvalidInputFileException(const std::string &message) : NeuralNetworkException(message) {}
};

struct InvalidComputationException : NeuralNetworkException {
    InvalidComputationException(const std::string &message) : NeuralNetworkException(message) {}
};


#endif //PV021_PROJECT_NEURALNETWORKEXCEPTIONS_H
