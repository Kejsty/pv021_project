//
// Created by Kejsty, Katarina Kejstova on 27.11.16.
//

#ifndef PV021_PROJECT_OUTPUTLAYERSNAPSHOT_H
#define PV021_PROJECT_OUTPUTLAYERSNAPSHOT_H


#include "Layer.h"

enum in {
    x1 = 0
    ,x2 = 1
    ,e = 2
}; //outputOrder

enum m {
    pi = 0,
    mu1 = 1,
    mu2 = 2,
    sig1 = 3,
    sig2 = 4,
    rho = 5

}; //mixturesOrder

class OutputLayerSnapshot {

    std::vector<double > nextInput;
    std::vector<double> mixtures;
    std::vector<double> _errors;

public:


    OutputLayerSnapshot( const std::vector<double> &mixtures ) : mixtures( mixtures ) , _errors(OSIZE) { }

    void setNextInput( const std::vector<double> &nextInput ) {
        OutputLayerSnapshot::nextInput = nextInput;
    }

    std::vector<double> evaluate() {
#if PRINT
        std::cout << "Counting  Output snapshot with values: x1:" << nextInput[in::x1] << " x2 :" << nextInput[in::x2] << std::endl ;
#endif
        assert(nextInput.size() > 0);
        double responsibilitiesSum = 0.0;
        std::vector<double> responsibilities;
        for ( int j = 0; j < 20 ; ++j ) {
            double probabilityDensity = countN(nextInput[in::x1], nextInput[in::x2], j) * mixtures[j*6 + 1];
            responsibilities.push_back(probabilityDensity);
            responsibilitiesSum += probabilityDensity;
        }
        assert(responsibilitiesSum != 0); ///IMPORTANT!!
        _errors[0] = nextInput[in::e] - mixtures[0]; // Eq 27
        std::transform(responsibilities.begin(), responsibilities.end(), responsibilities.begin(),
                       [&responsibilitiesSum](double responsibility){
                           return responsibility/responsibilitiesSum; // Eq 29
                       });


        for ( int j = 0; j < 20 ; ++j ) {

            int beg = j*6 + 1;
            double *currM = &(mixtures[beg]); //currentMixtures
            double C = 1 / (1 - pow(currM[m::rho], 2)); // see paper, Eq. 37
            _errors[beg + m::pi] = currM[m::pi] - responsibilities[j]; // Eq 30
            _errors[beg + m::mu1] = (C / currM[m::sig1])
                                    * (((nextInput[in::x1] - currM[m::mu1])/currM[m::sig1])
                                       - (currM[m::rho] * (nextInput[in::x2] - currM[m::mu2]) / currM[m::sig2]));
            _errors[beg + m::mu1] *= - _errors[beg + m::pi];

            _errors[beg + m::mu2] = (C / currM[m::sig2])
                                    * (((nextInput[in::x2] - currM[m::mu2])/currM[m::sig2])
                                       - (currM[m::rho] * (nextInput[in::x1] - currM[m::mu1]) / currM[m::sig1]));
            _errors[beg + m::mu2] *= - _errors[beg + m::pi];

            _errors[beg + m::sig1] = ((C * (nextInput[in::x1] - currM[m::mu1])) / currM[m::sig1])
                                    * (((nextInput[in::x1] - currM[m::mu1])/currM[m::sig1])
                                       - (currM[m::rho] * (nextInput[in::x2] - currM[m::mu2]) / currM[m::sig2])) - 1;
            _errors[beg + m::sig1] *= - _errors[beg + m::pi];

            _errors[beg + m::sig2] = ((C * (nextInput[in::x2] - currM[m::mu2])) / currM[m::sig2])
                                    * (((nextInput[in::x2] - currM[m::mu2])/currM[m::sig2])
                                       - (currM[m::rho] * (nextInput[in::x1] - currM[m::mu1]) / currM[m::sig1])) - 1;
            _errors[beg + m::sig2] *= - _errors[beg + m::pi];

            _errors[beg + m::rho] = ((nextInput[in::x1] - currM[m::mu1]) * (nextInput[in::x2] - currM[m::mu2]))
                                    / (currM[m::sig1] * currM[m::sig2])
                                    + currM[m::rho] * (1 - C * countZ(nextInput[in::x1], nextInput[in::x2], j));
            _errors[beg + m::rho] *= - _errors[beg + m::pi];
        }

        return _errors;
    }


    double countZ( double x1, double x2, int mixtureIter ) {

        int beg = mixtureIter*6+1; //real beggining position in mixtures acc mixtureIter
        double *currM = &(mixtures[beg]);
        double partA = pow((x1 - currM[m::mu1]), 2)/pow(currM[m::sig1],2);
        double partB =  pow((x2 - currM[m::mu2]), 2)/pow(currM[m::sig2],2);
        double partC = 2 * currM[m::rho] * (x1 - currM[m::mu1]) * (x2 - currM[m::mu2]) /
                       (currM[m::sig1]* currM[m::sig2]);
        double Z = partA + partB - partC;
//        std::cout << "x1 = " << x1 << " x2 = "<< x2 << " Z is " << Z << " ";
        return  Z;

    }

    double countN( double x1, double x2, int mixtureIter ) {

        int beg = mixtureIter*6+1; //real beggining position in mixtures acc mixtureIter
        double *currM = &(mixtures[beg]);
        double Z = countZ(x1,x2,mixtureIter);
        double partA = exp( (-Z) / ( 2  * (1 - pow(currM[m::rho],2))));
        double partB =  2 * M_PI * currM[m::sig1] * currM[m::sig2] * sqrt(1 - pow(currM[m::rho],2));
        double countedN = partA/ partB;
//        std::cout << "N is " << N << std::endl;
        return countedN;
    }


    const std::vector<double> &get_errors( ) const {
        return _errors;
    }
};


#endif //PV021_PROJECT_OUTPUTLAYERSNAPSHOT_H
