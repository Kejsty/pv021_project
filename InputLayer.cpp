//
// Created by Kejsty, Katarina Kejstova on 26.11.16.
//

#include <sstream>
#include "InputLayer.h"


//INPUT LAYER

InputLayer::InputLayer( const std::string &path ) : currentValues(3) , input(path){

    if (!input.good())
        throw InvalidInputFileException("Unable to open input file " + path);

    double x,y,e;
    std::string line;
    while (input.good()) {
        getline(input, line);
        std::stringstream buffer(line);
        buffer >> x >> y >> e;
        _x.push(x);
        _y.push(y);
        _e.push(e);
        if(e == 1.0)
            break;
    }
}

void InputLayer::loadData( const std::string &path ) {
    input.close();
    input.open(path);

    if (!input.good())
        throw InvalidInputFileException("Unable to open input file " + path);

    loadData();
}

void InputLayer::loadData( ) {
    double x,y,e;

    std::string line;
    while (input.good()) {
        getline(input, line);
        std::stringstream buffer(line);
        buffer >> x >> y >> e;
        _x.push(x);
        _y.push(y);
        _e.push(e);
        if(e == 1.0)
            break;
    }
}

bool InputLayer::eval( ) {

    currentValues[0] = _x.front( );
    _x.pop();
    currentValues[1] = _y.front( );
    _y.pop();
    currentValues[2] = _e.front( );
    _e.pop();
    if (currentValues[2] == 1) {
#if PRINT
        std::cout << "BackProp stared with values : : " << currentValues[0] << ", " << currentValues[1]  << ", " << currentValues[2] << std::endl;
#endif
        loadData();
        return true;
    }
    return false;
}





