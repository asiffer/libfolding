//
// Created by asr on 12/07/18.
//

#ifndef FOLDEDVARIANCE_H
#define FOLDEDVARIANCE_H

#include <armadillo>
#include <tbb/tbb.h>

class FoldedVariance {
    /**
     * This class aims to compute the folding variance in a
     * parallel way, through intel threading building block
     * (intel tbb)
     */
private:
    /**
     * Pointer to the observations
     */
    arma::mat *data_;

    /**
     * The folding ratio
     */
    arma::vec s_;

    /**
     * First output of the computation: the sum of the observations
     */
    double *sum_;

    /**
     * Second output of the computation: the sum of the squared observations
     */
    double *sum2_;

public:
    /**
     * Constructor
     * @param M Input observation (a d x n matrix)
     * @param s_star the pivot (a vector: d x 1 matrix)
     * @param sum a pointer to the sum of the observations
     * @param sum2 a pointer to the sum of the squared observations
     */
    FoldedVariance(arma::mat *M, arma::vec &s_star, double *sum, double *sum2);

    /**
     * Operator for the parallel compuation
     * @param r the range of the data taken into account
     */
    void operator()(const tbb::blocked_range<size_t> &r) const;

};


#endif //FOLDEDVARIANCE_H
