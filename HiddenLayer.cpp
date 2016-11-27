//
// Created by Kejsty, Katarina Kejstova on 26.11.16.
//

#include "HiddenLayer.h"

HiddenLayer::HiddenLayer(std::vector<Layer*> &&under) : innerValues(HIDDENLS),
                                                        c(HIDDENLS),
                                                        _underLayers(std::move(under)),
                                                        _weights(HIDDENLS)

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

    std::cout << "Step into: Hidden layer\n";
    //[x_t, h_underL, h_old]
    matrix values(1);
    values[0].reserve(_weights[0][0].size());
    for ( auto &layer : _underLayers ) {
        values[0].insert(values[0].end(),layer->getValues().begin(), layer->getValues().end());
    }

    std::vector<double> myError(HIDDENLS);
    std::fill(myError.begin(), myError.end(), 0.0);
    std::vector<matrix> _weightsErrorSumOverTime(HIDDENLS, (matrix (_weights[0].size(),std::vector<double>(_weights[0][0].size()))));

    std::cout << "Processing " << snapshots.size() << "snapshots\n";
    int position = 0;
    for ( auto it = snapshots.rbegin(); it != snapshots.rend(); it++ , ++position ) {

        std::cout << position << " : ";

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
            auto bla = algorithms::matrixMultiplication(transposedResults,values );
            algorithms::matrixSum(_weightsErrorSumOverTime[neuronId], bla );

        }
    }

    //TODO upravit vahy na zaklade sum _weightsErrorSumOverTime
    // to bude daky learning rate * toto, alebo daco podobne

    snapshots.clear();
}



