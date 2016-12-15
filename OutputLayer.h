//
// Created by Kejsty, Katarina Kejstova on 26.11.16.
//

#ifndef PV021_PROJECT_OUTPUTLAYER_H
#define PV021_PROJECT_OUTPUTLAYER_H

#include <map>
#include "Layer.h"
#include "OutputLayerSnapshot.h"

class OutputLayer : public Layer {
    std::vector<matrix>_weights;
    std::vector<Layer*> _underLayers;
    Layer* _inputLayer;
    std::vector<double> mixtures;
    std::map<Layer *, std::queue<std::vector<double>>> _errorMap;
    std::vector<OutputLayerSnapshot> snapshots;

public:

    double countMeanLogLossError();
    double countSumSquaredError() {
        double meanSnapshotError = 0.0;
        for ( auto &snapshot : snapshots) {
            meanSnapshotError += snapshot.countSnapshotError();
        }
        meanSnapshotError /= snapshots.size();
        snapshots.clear();
        std::cout << "Sequence Sum Squared Error: " << meanSnapshotError << std::endl;
        return meanSnapshotError;
    }
    virtual bool eval() override;
    virtual void backPropagate( const std::vector<double> & ) override;
    virtual void clear() override {
        snapshots.clear();
//        for (auto &record : _errorMap) {
//            assert(record.second.empty());
//        }
    }
    virtual size_t size() const override {
        return OSIZE;
    }
    virtual const std::vector<double> &getValues() const override {
        return mixtures;
    }

    OutputLayer( std::vector<Layer*> &&under, Layer *input) : _weights(under.size()),
                                                              _underLayers(std::move(under)),
                                                              _inputLayer(input),
                                                              mixtures(OSIZE)
    {
        std::uniform_real_distribution<double> unif(-W_INIT_MAX,W_INIT_MAX);
        std::default_random_engine re;
        for ( auto &layerWeights : _weights ) {
            layerWeights.resize(OSIZE);
            for ( auto &neuronWeights : layerWeights) {
                neuronWeights.resize(HSIZE);
                std::generate(neuronWeights.begin(), neuronWeights.end(), [&unif, &re](){ return unif(re);});
            }
            for (int stdDevPosition = 1; stdDevPosition < 121; stdDevPosition += 6) {
                // so the std devs are initialized to 0.075
                mixtures[stdDevPosition + m::sig1] = -2.59027;
                mixtures[stdDevPosition + m::sig2] = -2.59027;
            }
        }

        std::generate(mixtures.begin(), mixtures.end(), [&unif, &re](){ return unif(re);});
        for (auto &layer : _underLayers)  {
            _errorMap.insert(std::pair<Layer *, std::queue<std::vector<double>>>(layer, std::queue<std::vector<double>>()));
        }
    }

    virtual const std::vector<double> getMyErrors( Layer *me ) override {
        auto it = _errorMap.find(me);
        assert (it != _errorMap.end());
        assert (it->second.size() > 0);
        auto res = it->second.front();
        it->second.pop();
        return res;
    }

};




#endif //PV021_PROJECT_OUTPUTLAYER_H
