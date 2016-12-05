//
// Created by Kejsty, Katarina Kejstova on 15.11.16.
//

#include "algorithms.h"
#include <cassert>
#include <cstdio>
#include <iostream>
#include <numeric>
#include <math.h>

namespace algorithms {
    matrix matrixMultiplication(const matrix &fst, const matrix &snd) {
            assert( fst.size( ) > 0 );
            assert( snd.size( ) > 0 );
            assert( snd[ 0 ].size( ) > 0 );
            assert( fst[ 0 ].size( ) == snd.size( ));

            matrix newMatrix( fst.size( ), std::vector<double>( snd[ 0 ].size( )));
            for ( size_t i = 0; i < fst.size( ); ++i ) {
                    for ( size_t j = 0; j < snd[ 0 ].size( ); ++j ) {
                            newMatrix[ i ][ j ] = 0;
                            for ( size_t k = 0; k < snd.size( ); ++k )
                                    newMatrix[ i ][ j ] += ( fst[ i ][ k ] * snd[ k ][ j ] );
                    }

            }
            return newMatrix;
    }


    matrix transposeMatrix(const matrix &m) {

        matrix outMatrix(m[0].size(), std::vector<double>(m.size()));
        size_t size = m.size();
        for (size_t x = 0; x < size; ++x) {
            for ( size_t y = 0; y < m[ 0 ].size( ); ++y ) {
                outMatrix[ y ][ x ] = m[ x ][ y ];
            }
        }
        return  outMatrix;
    }


    matrix matrixSum( matrix &fst, const matrix &snd) {
        assert( fst.size( ) > 0 );
        assert( snd.size( ) > 0 );
        assert( snd.size() == fst.size() );
        assert( fst[ 0 ].size( ) == snd[0].size( ));

        for ( size_t i = 0; i < fst.size() ; ++i ) {
            std::transform(fst[i].begin(), fst[i].end(), snd[i].begin(), fst[i].begin(), std::plus<double >());
        }

        return fst;

    }

    double sigmoid( double x)
    {
        double exp_val;

        exp_val = exp((double) -x);

        return 1 / (1 + exp_val);
    }
}
