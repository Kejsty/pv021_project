//
// Created by Kejsty, Katarina Kejstova on 26.11.16.
//

#include "HiddenLayer.h"

HiddenLayer::HiddenLayer(std::vector<Layer*> &&under) : innerValues(HSIZE),
                                                        c(HSIZE),
                                                        _underLayers(std::move(under)),
                                                        _weights(HSIZE)

{
    //i'm in my own _underLayers - because of loop on every hidden layer
    _underLayers.push_back(this);
    for (auto &layer : _underLayers)  {
        _errorMap.insert(std::pair<Layer *, std::queue<std::vector<double>>>(layer, std::queue<std::vector<double>>()));
    }

    //random number generator in interval <0,W_INIT_MAX>
    std::uniform_real_distribution<double> unif(-W_INIT_MAX,W_INIT_MAX);
    std::default_random_engine re;

    size_t _weightsLength = std::accumulate(_underLayers.begin(), _underLayers.end(), static_cast<size_t> (0),
                                            [](size_t partialResult, const Layer* layer){
                                                return partialResult + layer->size();
                                            });


    //Wi, Wf, Wo, Wc for every cell
    for (auto &neuronWeights : _weights) {
        neuronWeights.resize(4);
        for (auto &weight : neuronWeights) {
            weight.resize(_weightsLength);
            std::generate(weight.begin(), weight.end(), [&unif, &re]( ) { return unif(re);});
        }
    }

    std::generate(c.begin(), c.end(), [&unif, &re]( ) { return unif(re);});

    std::generate(innerValues.begin(), innerValues.end(), [&unif, &re]( ) { return unif(re);});

}

bool HiddenLayer::eval( ) {

    std::vector<double> values;
    values.reserve(_weights[0][0].size());
    for ( auto &layer : _underLayers ) {
        values.insert(values.end(),layer->getValues().begin(), layer->getValues().end());
    }


    //count that inner neuron shit
    std::vector<double> oldC(c);
    std::vector<double> partialC , _i, _f, _o, _c;


    std::transform(_weights.begin(), _weights.end(), c.begin(),  innerValues.begin(),
                    // Wi Ui Vi
                    // Wf Uf Vf
                    // Wo Uo Vo
                    // Wc Uc Vc
                   [&](const matrix& neuronWeights, double c_old){
                       std::vector<double> partialValues(4);
                       std::transform(neuronWeights.begin(), neuronWeights.end(), partialValues.begin(),
                                      [&]( const std::vector<double> &gateWeights) {
                                          return std::inner_product(gateWeights.begin(), gateWeights.end(), values.begin(), 0.0 );
                                      });

                       partialValues[v::i] = algorithms::sigmoid(partialValues[v::i]);
                       _i.push_back(partialValues[v::i]);

                       partialValues[v::f] = algorithms::sigmoid(partialValues[v::f]);
                       _f.push_back(partialValues[v::f]);

                       partialC.push_back(partialValues[v::c]);
                       partialValues[v::c] = partialValues[v::i] * tanh(partialValues[v::c]) + partialValues[v::f] * c_old;
                       _c.push_back(partialValues[v::c]);

                       partialValues[v::o] = algorithms::sigmoid(partialValues[v::o]);
                       _o.push_back(partialValues[v::o]);

                       return partialValues[v::o] * tanh(partialValues[v::c]);
                   });

    snapshots.push_back(std::move(HiddenLayerSnapshot(std::move(_i), std::move(_f), std::move(_o), std::move(_c),
                                                      std::move(partialC), std::move(oldC))));
    return false;
}


void HiddenLayer::backPropagate( const std::vector<double> & ) {

//    std::cout << "Step into: Hidden layer\n";
    //[x_t, h_underL, h_old]
    matrix values(1);
    values[0].reserve(_weights[0][0].size());
    for ( auto &layer : _underLayers ) {
        values[0].insert(values[0].end(),layer->getValues().begin(), layer->getValues().end());
    }

    std::vector<double> myError(HSIZE);
    std::fill(myError.begin(), myError.end(), 0.0);
    std::vector<matrix> _weightsErrorSumOverTime(HSIZE, (matrix (_weights[0].size(),std::vector<double>(_weights[0][0].size()))));
    std::vector<matrix> _weightsErrorGOverTime(HSIZE, (matrix (_weights[0].size(),std::vector<double>(_weights[0][0].size()))));
    std::vector<matrix> _weightsErrorNOverTime(HSIZE, (matrix (_weights[0].size(),std::vector<double>(_weights[0][0].size()))));
#if PRINT
    std::cout << "Processing " << snapshots.size() << "snapshots\n";
#endif
    int position = 0;
    for ( auto it = snapshots.rbegin(); it != snapshots.rend(); it++ , ++position ) {

#if PRINT
        std::cout << position << " : ";
#endif
        //get my counted errors from layers above in this iteration
        std::fill(myError.begin(), myError.end(), 0.0);
        for (auto &layer : _aboveLayers) {
            auto propagatedLayers = layer->getMyErrors(this);
            if (propagatedLayers.size() != 0) {
                std::transform( myError.begin( ), myError.end( ), propagatedLayers.begin( ), myError.begin( ),
                                std::plus<double>( ));
            }
        }

        //count my current error from my snapshot
        matrix weigthsForErrMap(_underLayers.size());
        matrix result = it->evaluate(myError);
        for ( size_t neuronId = 0; neuronId < _weights.size(); ++neuronId ) {

            //get snapshot errors in needed form
            std::vector<double > myResult;
            for ( size_t gateId = 0; gateId < result.size( ); ++gateId ) {
                myResult.push_back(result[gateId][neuronId]);
            }

            //transposed weights * snapshot errors -> errors for layers under me
            for ( size_t layerId = 0; layerId < _underLayers.size(); ++layerId ) {
                auto transposed = algorithms::transposeMatrix(_weights[neuronId]);
                weigthsForErrMap[layerId].push_back(
                        std::inner_product(transposed[layerId].begin(), transposed[layerId].end(), myResult.begin(), 0 ));
            }

            //store counted error for me, and layers under me
            for ( size_t layerId = 0; layerId < _underLayers.size(); ++layerId ) {
                _errorMap.find(_underLayers[layerId])->second.push(weigthsForErrMap[layerId]);
            }

            //snapshot errors * current input values -> my weights erros, store it for weights adaptation at the end
            matrix resultAsMatrix(1, myResult);
            auto transposedResults = algorithms::transposeMatrix(resultAsMatrix);
            auto currentWeightError = algorithms::matrixMultiplication(transposedResults,values );
            for ( int gateId = 0; gateId < 4; ++gateId ) {
                std::transform(_weightsErrorNOverTime[neuronId][gateId].begin(), _weightsErrorNOverTime[neuronId][gateId].end(), currentWeightError[gateId].begin(), _weightsErrorNOverTime[neuronId][gateId].begin(),
                               [](double oldN, double currentError) {
                                   return oldN * N + (1 - N)* pow(currentError, 2);
                               });

                std::transform(_weightsErrorGOverTime[neuronId][gateId].begin(), _weightsErrorGOverTime[neuronId][gateId].end(), currentWeightError[gateId].begin(), _weightsErrorGOverTime[neuronId][gateId].begin(),
                               [](double oldG, double currentError) {
                                   return oldG * N + (1 - N)* currentError;
                               });

                for ( size_t inputId = 0; inputId < currentWeightError[gateId].size(); ++inputId ) {
                    _weightsErrorSumOverTime[neuronId][gateId][inputId] = BIGU * _weightsErrorSumOverTime[neuronId][gateId][inputId] -
                                                                         SMALLU * currentWeightError[gateId][inputId] / sqrt(_weightsErrorNOverTime[neuronId][gateId][inputId] -
                                                                                                                             pow(_weightsErrorGOverTime[neuronId][gateId][inputId],
                                                                                                                                       2) + LAMPA);
                }
            }

        }
    }

    for ( int neuronId = 0; neuronId < HSIZE; ++neuronId ) {
        algorithms::matrixSum(_weights[neuronId], _weightsErrorSumOverTime[neuronId]);
    }

    snapshots.clear();
}



