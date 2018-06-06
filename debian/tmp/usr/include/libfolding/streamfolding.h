//
// Created by asr on 01/06/18.
//

#ifndef STREAMFOLDING_H
#define STREAMFOLDING_H

#include <iostream>
#include "pivot.h"
#include "armaring.h"

typedef double (*pvalfun)(double, double);

using namespace std;

/* These constants had been computed
 * through Monte-Carlo simulation */
#define pval_a 0.4785
#define pval_b 0.1946
#define pval_c 2.0287


class StreamFolding {
private:
    /**
     * The dimension of the observations
     */
    size_t dimension_;

    /**
     * Total number of iterations made by the algorithm
     */
    size_t iteration_;

    /**
     * The number of stored observations (size of the sliding window)
     */
    size_t depth_;

    /**
     * A basic indicator of the Pivot initialization
     */
    bool initialized_;

    /**
     * Object to compute the pivot in a streaming fashion
     */
    Pivot *pivot_;

    /**
     * Data container
     */
    ArmaRing ring_;


public:
    /**
     * Constructor
     * @param depth The size of the sliding window
     */
    StreamFolding(size_t depth = 0);

    ~StreamFolding();

    /**
     * Main method (update the internal state according to a new observation)
     * @param v A new observation
     */
    void update(arma::vec v);

    /**
     * Check is the Pivot has bee initialized
     * @return
     */
    bool is_initialized();

    /**
     * Compute the mean of the stored data (use the function of the Pivot object)
     * @return
     */
    arma::mat mean();

    /**
     * Compute the covariance of the stored data (use the function of the Pivot object)
     * @return
     */
    arma::mat cov();

    /**
     * Compute the pivot s2*
     * @return
     */
    arma::vec s2star();

    /**
     * Perform the folding test of unimodality
     * @param unimodal a pointer to the result of the test (true: unimodal, false: multimodal)
     * @param p_value a pointer to the significance of the test (the lower, the better,
     * usually p_value < 0.05 is used as a decision threshold
     * @return the folding statistics (>=1 means rather unimodal, <1 means rather multimodal)
     */
    double folding_test(bool *unimodal, double *p_value);

    /**
     * Return a dump of the stored data as a d x n matrix where n is the current number of
     * stored data (equal to the window size in the cruising regime)
     * @return
     */
    arma::mat dump();

};


#endif // STREAMFOLDING_H
