//
// Created by Kejsty, Katarina Kejstova on 26.11.16.
//

#ifndef PV021_PROJECT_HIDDENLAYER_H
#define PV021_PROJECT_HIDDENLAYER_H

#include <map>
#include "Layer.h"
#include "HiddenLayerSnapshot.h"
#include "InputLayer.h"

class HiddenLayer : public Layer {

    std::vector<double> innerValues; //h_t
    std::vector<double> c;
    std::vector<Layer*> _aboveLayers; //for backProp?
    std::vector<Layer*> _underLayers;
    //Wi, Wf, Wo, Wc...
    std::vector<matrix> _weights;
    std::map<Layer *, std::queue<std::vector<double>>> _errorMap;

    std::vector<HiddenLayerSnapshot> snapshots;

public:

    virtual ~HiddenLayer( ) { }

    HiddenLayer(std::vector<Layer*> &&under);

    virtual bool eval() override;

    virtual void backPropagate( const std::vector<double> &initError ) override;

    virtual void clear() override {
        snapshots.clear();
        for (auto &record : _errorMap) {
            //Input Layer don't use it's error, and
            // this Hidden Layer don't need it's last counted error
            if ( dynamic_cast<InputLayer *>(record.first) || record.first == this) {
                std::queue<std::vector<double>> empty;
                record.second = empty;
            }else {
                assert(record.second.empty());
            }
        }
    }

    size_t size() const override {
        return HSIZE;
    }

    void setAbove(Layer* above) {
        _aboveLayers.push_back(above);
    }

    const std::vector<double> &getValues() const override {
        //because last values are our's X
        return innerValues;
    }

    virtual const std::vector<double> getMyErrors( Layer *me ) override {
        auto it = _errorMap.find(me);
        if (me == this &&  it->second.size() == 0) {
            return {};
        }
        assert (it != _errorMap.end());
        assert (it->second.size() > 0);
        auto res = it->second.front();
        it->second.pop();
        return res;
    }

};



#endif //PV021_PROJECT_HIDDENLAYER_H
