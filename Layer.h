//
// Created by Kejsty, Katarina Kejstova on 3.11.16.
//

#ifndef PV021_PROJECT_LAYER_H
#define PV021_PROJECT_LAYER_H


#include <vector>
#include <iostream>
#include <array>
#include <cassert>
#include <cstdio>
#include <queue>
#include "algorithms.h"
#include <random>
#include <algorithm>
#include <cmath>

#define HIDDENLS 400
#define W_INIT_MAX 0.5
#define OUTPUTS 121
#define IMAGEX 20
#define IMAGEY 20

using matrix = std::vector<std::vector<double>>;

enum v {
    i = 0
    ,f = 1
    ,o = 2
    ,c = 3
    ,c_part = 4
    ,c_old = 5
};


//interface
class Layer {
public:
    virtual bool eval() = 0;
    virtual void backPropagate( const std::vector<double> &initError ) = 0;
    virtual size_t size() const = 0;
    virtual const std::vector<double> &getValues() const  = 0;
    virtual const std::vector<double> getMyErrors( Layer *me )  = 0;
};




#endif //PV021_PROJECT_LAYER_H
