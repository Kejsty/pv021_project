//
// Created by Kejsty, Katarina Kejstova on 15.11.16.
//

#ifndef PV021_PROJECT_ALGORITHMS_H
#define PV021_PROJECT_ALGORITHMS_H

#include <vector>

using matrix = std::vector<std::vector<double>>;
namespace algorithms {
    matrix matrixMultiplication( const matrix &fst, const matrix &snd );

    matrix transposeMatrix(const matrix &m);

    matrix matrixSum( matrix &fst, const matrix &snd);

    double sigmoid( double x);
}

#endif //PV021_PROJECT_ALGORITHMS_H
