//
// Created by Kejsty, Katarina Kejstova on 26.11.16.
//

#ifndef PV021_PROJECT_INPUTLAYER_H
#define PV021_PROJECT_INPUTLAYER_H

#include "Layer.h"

//just holds information about current x,y,e input
class InputLayer : public Layer {
    std::vector<double> currentValues; //x,y,e, e must be represented as double
    std::vector<double> realValues; //x,y,e, e must be represented as double
    std::queue<double, std::deque<double>> _x, _y, _e;
public:

    InputLayer( const std::vector<double> &x, const std::vector<double> &y, const std::vector<double> &e );

    InputLayer( const std::string &path );

    //must be implemented!
    virtual bool eval() override {
        currentValues[0] = _x.front( ) - realValues[0];
        realValues[0] = _x.front();
        _x.pop();
        currentValues[1] = _y.front( ) - realValues[1];
        realValues[1] = _y.front();
        _y.pop();
        currentValues[2] = _e.front( );
        _e.pop();
        if (currentValues[2] == 1) {
            std::cout << "BackProp stared with values : : " << currentValues[0] << ", " << currentValues[1]  << ", " << currentValues[2] << std::endl;
            return true;
        }
#if PRINT
        std::cout << "current input values : " << currentValues[0] << ", " << currentValues[1]  << ", " << currentValues[2] << std::endl;
#endif
        return false;
    }
    virtual void backPropagate( const std::vector<double> & ) override {
        std::cout << "Step into: Input layer\n BackProp end \n";
    }
    const std::vector<double> &getValues() const override {
        return currentValues;
    }

    bool hasValues() {
        return _x.size() > 0;
    }

    void reset() {
        realValues[0]= _x.empty() ? 0 : _x.front();
        realValues[1]= _y.empty() ? 0 : _y.front();
    }

    virtual const std::vector<double> getMyErrors( Layer * )  override { return {}; } //will not be called!

    size_t size() const override {
        return  3;
    }

};



#endif //PV021_PROJECT_INPUTLAYER_H
