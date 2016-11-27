//
// Created by Kejsty, Katarina Kejstova on 26.11.16.
//

#include <fstream>
#include "InputLayer.h"


//INPUT LAYER

InputLayer::InputLayer( const std::vector<double> &x, const std::vector<double> &y,
                        const std::vector<double > &e ) : currentValues(3),
                                                          _x( std::deque<double>( x.begin( ), x.end( ))),
                                                          _y( std::deque<double>( y.begin( ), y.end( ))),
                                                          _e( std::deque<double>( e.begin( ), e.end( ))) {
    assert( x.size() == y.size());
    assert( x.size() == e.size());
    currentValues[0] = _x.front( );
    currentValues[1] = _y.front( );
    currentValues[2] = static_cast<double>(_e.front( ));
}

InputLayer::InputLayer( const std::string &path ) : currentValues(3), realValues(2) {
    std::ifstream fileStream(path);
    double x,y,e;
    while (fileStream.good()) {
        fileStream >> x >> y >> e;
        _x.push(x);
        _y.push(y);
        _e.push(e);
    }
    realValues[0]= _x.front();
    realValues[1]= _y.front();
}

