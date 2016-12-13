//
// Created by Kejsty, Katarina Kejstova on 26.11.16.
//

#ifndef PV021_PROJECT_INPUTLAYER_H
#define PV021_PROJECT_INPUTLAYER_H

#include <fstream>
#include "Layer.h"

//just holds information about current x,y,e input
class InputLayer : public Layer {
    std::vector<double> currentValues; //x,y,e, e must be represented as double
    std::queue<double, std::deque<double>> _x, _y, _e;
    std::fstream input;
public:

    InputLayer( const std::string &path );

    //must be implemented!
    virtual bool eval() override;
    virtual void backPropagate( const std::vector<double> & ) override {
        std::cout << "BackProp end \n";
    }
    virtual void clear() override {}
    const std::vector<double> &getValues() const override {
        return currentValues;
    }

    bool hasValues() {
        return _x.size() > 0;
    }

    bool isEndOfSequence() {
        return _x.empty() || (_x.front() == 0 && _y.front() == 0 && _e.front() == 0);
    }

    virtual const std::vector<double> getMyErrors( Layer * )  override { return {}; } //will not be called!

    size_t size() const override {
        return  3;
    }

    void loadData(const std::string &path);
    void loadData();

};



#endif //PV021_PROJECT_INPUTLAYER_H
