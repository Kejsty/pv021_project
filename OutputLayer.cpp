//
// Created by Kejsty, Katarina Kejstova on 26.11.16.
//

#include "OutputLayer.h"


bool OutputLayer::eval( ) {

   if (snapshots.size() > 0) {
       snapshots[snapshots.size() - 1].setNextInput(_inputLayer->getValues());
   }

    for ( int neuronID = 0; neuronID < OSIZE; ++neuronID ) { mixtures[neuronID] = 0.0; }

    for ( size_t layerId = 0; layerId < _underLayers.size(); ++layerId ) {
        for ( int neuronID = 0; neuronID < OSIZE; ++neuronID ) {
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

    for ( int i = 1; i < OSIZE; i += 6 ) {

        //mixtureWeights
        mixtures[i+m::pi] = exp(mixtures[i+m::pi]);
        mixtureWeightsSum += mixtures[i+m::pi];

        //means stay

        //standart deviations
        mixtures[i+m::sig1] = exp(mixtures[i+m::sig1]);
        if(mixtures[i+m::sig1] <= 0) {
            std::cerr << mixtures[i+m::sig1] << std::endl;
        }
        assert(mixtures[i+m::sig1] > 0);
        mixtures[i+m::sig2] = exp(mixtures[i+m::sig2]);
        if(mixtures[i+m::sig2] <= 0) {
            std::cerr << mixtures[i+m::sig2 ] << std::endl;
        }
        assert(mixtures[i+m::sig2] > 0);

        //correlations
        mixtures[i+m::rho] = tanh(mixtures[i+5]);
        assert(mixtures[i+m::rho] > -1 &&  mixtures[i+m::rho] < 1);
    }


    for ( int i = 1; i < OSIZE; i += 6 ) {
        //mixtureWeights normalization
        mixtures[i] /= mixtureWeightsSum;
    }

    //store this result for backProp
    snapshots.push_back(std::move(OutputLayerSnapshot(mixtures)));
    return false;
}

void OutputLayer::backPropagate( const std::vector<double> &lastInput ) {
#if PRINT
    std::cout << "Step into: Hidden layer\n";
#endif
    snapshots[snapshots.size() - 1].setNextInput(lastInput);

    //to modify weights at the end
    std::vector<matrix> _weightsErrorSumOverTime(HSIZE, (matrix (_weights[0].size(), std::vector<double>(_weights[0][0].size()))));
    std::vector<matrix> _weightsErrorGOverTime(HSIZE, (matrix (_weights[0].size(), std::vector<double>(_weights[0][0].size()))));
    std::vector<matrix> _weightsErrorNOverTime(HSIZE, (matrix (_weights[0].size(), std::vector<double>(_weights[0][0].size()))));

    //count my current error from my snapshot
    matrix weightsForErrMap(_underLayers.size());
#if PRINT
    std::cout << "Processing " << snapshots.size() << "snapshots\n";
#endif
    int position = 0;
    for ( auto it = snapshots.rbegin(); it != snapshots.rend(); ++it , ++position) {
#if PRINT
        std::cout << position << " : ";
#endif
        std::vector<double> result = it->evaluate();
        if ( position == 0) {
            std::cout << "Output errors: \n";
            for (auto &error : result) {
                std::cout << error << ",  ";
            }
            std::cout << std::endl;
        }

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
            auto currentWeightError = algorithms::matrixMultiplication(transposedResults, inputAsMatrix);
            for ( int outputId = 0; outputId < OSIZE; ++outputId ) {
                std::transform(_weightsErrorNOverTime[layerId][outputId].begin(), _weightsErrorNOverTime[layerId][outputId].end(), currentWeightError[outputId].begin(), _weightsErrorNOverTime[layerId][outputId].begin(),
                               [](double oldN, double currentError) {
                                   return oldN * N + (1 - N)* pow(currentError, 2);
                               });

                std::transform(_weightsErrorGOverTime[layerId][outputId].begin(), _weightsErrorGOverTime[layerId][outputId].end(), currentWeightError[outputId].begin(), _weightsErrorGOverTime[layerId][outputId].begin(),
                               [](double oldG, double currentError) {
                                   return oldG * N + (1 - N)* currentError;
                               });

                for ( size_t neuronID = 0; neuronID < HSIZE; ++neuronID ) {
                    _weightsErrorSumOverTime[layerId][outputId][neuronID] = BIGU * _weightsErrorSumOverTime[layerId][outputId][neuronID] -
                                                                  SMALLU * currentWeightError[outputId][neuronID] / sqrt(_weightsErrorNOverTime[layerId][outputId][neuronID] -
                                                                                                         pow(_weightsErrorGOverTime[layerId][outputId][neuronID],
                                                                                               2) + LAMPA);
                }
            }
        }

    }
    for ( size_t layerId = 0; layerId < _underLayers.size(); ++layerId ) {
        algorithms::matrixSum(_weights[layerId], _weightsErrorSumOverTime[layerId]);
    }

    //test that my output is OK
    for ( auto &item : _errorMap) {
        assert(item.second.size() == snapshots.size());
        assert(item.second.front().size() == HSIZE);
    }

    snapshots.clear();
}


