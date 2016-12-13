//
// Created by Kejsty, Katarina Kejstova on 26.11.16.
//

#ifndef PV021_PROJECT_HIDDENLAYERSNAPSHOT_H
#define PV021_PROJECT_HIDDENLAYERSNAPSHOT_H


#include <vector>
#include <algorithm>
#include "Layer.h"


class HiddenLayerSnapshot {
    matrix values; // order : i, f, o, c,c_partial, c_old;
    matrix errors; //order : i,f, o, c


public:
    HiddenLayerSnapshot(  std::vector<double> &&i,  std::vector<double> &&f, std::vector<double> &&o,
                          std::vector<double> &&c,  std::vector<double> &&c_partial,
                          std::vector<double> &&c_old ) : values(6), errors(4, std::vector<double>(i.size())) {
//        assert(i.size() > 0);
//        assert(i.size() ==  f.size());
//        assert(i.size() ==  o.size());
//        assert(i.size() ==  c.size());
//        assert(i.size() ==  c_partial.size());
//        assert(i.size() ==  c_old.size());
        values[0] = std::move(i);
        values[1] = std::move(f);
        values[2] = std::move(o);
        values[3] = std::move(c);
        values[4] = std::move(c_partial);
        values[5] = std::move(c_old);
    }

    matrix evaluate(const std::vector<double> &hError) {
        for ( size_t neuronId = 0; neuronId < values.size( ); ++neuronId ) {
            double iErr, fErr, cErr, oErr, c_partErr;
            // val order : i, f, o, c,c_partial, c_old;

            oErr = hError[neuronId] * tanh(values[v::c][neuronId]);
            cErr = hError[neuronId] * values[v::o][neuronId] * ( 1 - pow(tanh(values[v::c][neuronId]),2));
            iErr = cErr * tan(values[v::c_part][neuronId]);
            fErr = cErr * values[v::c_old][neuronId];
            c_partErr = cErr * values[v::i][neuronId];

            std::vector<double> toReturn(4);
            errors[v::i][neuronId] = iErr * values[v::i][neuronId] * (1 - values[v::i][neuronId]);
            errors[v::f][neuronId] = fErr * values[v::f][neuronId] * (1 - values[v::f][neuronId]);
            errors[v::o][neuronId] =  oErr * values[v::o][neuronId] * ( 1 - values[v::o][neuronId]);
            errors[v::c][neuronId] = c_partErr * ( 1 - pow(tanh(values[v::c_part][neuronId]),2) );
        }
        return errors;
    }


public:
    const matrix &getErrors( ) const {
        return errors;
    }
};


#endif //PV021_PROJECT_HIDDENLAYERSNAPSHOT_H
