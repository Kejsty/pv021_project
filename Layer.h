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
#include "NeuralNetworkExceptions.h"

#define HSIZE 400
#define W_INIT_MAX 0.5
#define OSIZE 121
#define ALEF 0.95
#define BET 0.9
#define NUN 0.0001
#define DALET 0.0001
#define PRINT 1

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
    virtual void clear() = 0;
    virtual size_t size() const = 0;
    virtual const std::vector<double> &getValues() const  = 0;
    virtual const std::vector<double> getMyErrors( Layer *me )  = 0;
};




#endif //PV021_PROJECT_LAYER_H
