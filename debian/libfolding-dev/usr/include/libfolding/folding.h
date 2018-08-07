//
// Created by asr on 01/06/18.
//

#ifndef FOLDING_H
#define FOLDING_H

#include "streamfolding.h"
#include <iomanip>
#include <chrono>



double significance(double Phi, size_t n, size_t d);


using T = std::chrono::steady_clock::time_point;
using usec = std::chrono::microseconds;
using sec = std::chrono::seconds;

class BatchFolding {
public:
    size_t n;
    size_t d;
    bool unimodal;
    double Phi;
    double p_value;
    arma::vec s2star;
    long int usec;

    BatchFolding(arma::mat M);

    void print();
};


#endif // FOLDING_H
