//
// Created by asr on 01/06/18.
//

#ifndef ARMARING_H
#define ARMARING_H

#include <armadillo>
#include "tbb/tbb.h"

using namespace std;


class ArmaRing {
private:
    /**
     * The current cursor (index where to insert the next data)
     */
    size_t cursor_;

    /**
     * Maximum size of the ring
     */
    size_t max_size_;

    /**
     * The real data container (a matrix)
     */
    arma::mat memory_;

    /**
     * the last erased data
     */
    arma::vec last_erased_;


public:
    /**
     * The status of the container
     */
    enum STATUS : int {
        INFINITE = -1,
        UNFILLED = 1,
        FILLED = 0,
        ERROR = -3
    };

    /**
     * Constructor
     * @param max_size the size of the container (its size will grow from zero to max_size)
     */
    ArmaRing(size_t max_size);

    /**
     * The main method, which update the container by inserting a new observation
     * in the index given by cursor_ (it may remove the previous data at this index)
     * @param v A new observation
     * @return The final status of the container
     */
    int update(arma::vec v);

    /**
     * Return the current size of the container (the current
     * number of stored data)
     * @return
     */
    size_t size();

    /**
     * Compute the folded variance Var|X-s2_star| where X is the stored data (memory_) and
     * s2_star an external parameter (called "pivot")
     * @param s2_star The folding pivot
     * @return Var|X-s2_star|
     */
    double folded_var(arma::vec s2_star);

    /**
     * Dump the current memory (not ranked)
     * @return
     */
    arma::mat dump();

    /**
     * Get the last erased data
     * @return
     */
    arma::vec get_last_erased();
};


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
    void operator()(const tbb::blocked_range <size_t> &r) const;

};

#endif // ARMARING_H
