//
// Created by Kejsty, Katarina Kejstova on 26.11.16.
//

#include "OutputLayer.h"


bool OutputLayer::eval( ) {

   if (snapshots.size() > 0) {
       snapshots[snapshots.size() - 1].setNextInput(_inputLayer->getValues());
   }

    for ( int neuronID = 0; neuronID < OUTPUTS; ++neuronID ) { mixtures[neuronID] = 0.0; }

    for ( size_t layerId = 0; layerId < _underLayers.size(); ++layerId ) {
        for ( int neuronID = 0; neuronID < OUTPUTS; ++neuronID ) {
            mixtures[neuronID] += std::inner_product(_weights[layerId][neuronID].begin(),
                                                     _weights[layerId][neuronID].end(),
                                                     _underLayers[layerId]->getValues().begin(), 0.0);
        }
    }

    //TODO : do something with the sum?? some kind of activation function?

    //count mixtures
    //e_t
    auto e_t = mixtures[0];
    mixtures[0] = 1/(1+exp(e_t));

    double mixtureWeightsSum = 0.0;

    for ( int i = 1; i < OUTPUTS; i += 6 ) {

        //mixtureWeights
        mixtures[i] = exp(mixtures[i]);
        mixtureWeightsSum += mixtures[i];

        //means stay

        //standart deviations
        mixtures[i+3] = exp(mixtures[i+3]);
        assert(mixtures[i+3] > 0);
        mixtures[i+4] = exp(mixtures[i+4]);
        assert(mixtures[i+4] > 0);

        //correlations
        mixtures[i+5] = tanh(mixtures[i+5]);
        assert(mixtures[i+5] > -1 &&  mixtures[i+5] < 1);
    }


    for ( int i = 1; i < OUTPUTS; i += 6 ) {
        //mixtureWeights normalization
        mixtures[i] /= mixtureWeightsSum;
    }

    //store this result for backProp
    snapshots.push_back(std::move(OutputLayerSnapshot(mixtures)));
    return false;
}

void OutputLayer::backPropagate( const std::vector<double> &lastInput ) {

    std::cout << "Step into: Hidden layer\n";
    snapshots[snapshots.size() - 1].setNextInput(lastInput);

    //to modify weights at the end
    std::vector<matrix> _weightsErrorSumOverTime(HIDDENLS, (matrix (_weights[0].size(),std::vector<double>(_weights[0][0].size()))));

    //count my current error from my snapshot
    matrix weightsForErrMap(_underLayers.size());
    std::cout << "Processing " << snapshots.size() << "snapshots\n";
    int position = 0;
    for ( auto it = snapshots.rbegin(); it != snapshots.rend(); ++it , ++position) {
        std::cout << position << " : ";

        std::vector<double> result = it->evaluate();

        //transposed weights * snapshot errors -> errors for layers under me
        for ( size_t layerId = 0; layerId < _underLayers.size(); ++layerId ) {
            matrix transposedWeights = algorithms::transposeMatrix(_weights[layerId]);
            for (size_t neuronId = 0; neuronId < _underLayers[layerId]->size(); ++neuronId) {
                double a = std::inner_product(transposedWeights[neuronId].begin(), transposedWeights[neuronId].end(), result.begin(), 0.0 );
                weightsForErrMap[layerId].push_back(a);
            }
        }

        //store counted error for me, and layers under me
        for ( size_t layerId = 0; layerId < _underLayers.size(); ++layerId ) {
            _errorMap.find(_underLayers[layerId])->second.push(weightsForErrMap[layerId]);
        }

        //snapshot errors * current input values -> my weights error
        matrix resultAsMatrix(1, result);
        auto transposedResults = algorithms::transposeMatrix(resultAsMatrix);
        for ( size_t layerId = 0; layerId < _underLayers.size(); ++layerId ) {
            matrix inputAsMatrix (1,_underLayers[i]->getValues());
            algorithms::matrixSum(_weightsErrorSumOverTime[layerId], algorithms::matrixMultiplication(transposedResults, inputAsMatrix));
        }

        //TODO upravit vahy na zaklade sum _weightsErrorSumOverTime
        // to bude daky learning rate * toto, alebo daco podobne
    }

    //test that my output is OK
    for ( auto &item : _errorMap) {
        assert(item.second.size() == snapshots.size());
        assert(item.second.front().size() == HIDDENLS);
    }

    snapshots.clear();
}


