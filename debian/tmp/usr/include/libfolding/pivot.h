//
// Created by asr on 31/05/18.
//

#ifndef PIVOT_H
#define PIVOT_H

#include <armadillo>

using namespace std;


class Pivot {
private:
    /**
     * The number of observations
     */
    size_t n;

    /**
     * The sum of the observations
     */
    arma::vec S_X;

    /**
    * The sum of Xi Xi' (Xi is an observation)
    */
    arma::mat S_X2;

    /**
     * The sum of the squared norms of the observations
     */
    double S_R;

    /**
     * The covariance between the observations and their norm: cov(X,|X|²) ?
     */
    arma::vec V_XR;

    /**
     * The inverse of XX' where X is the d x n matrice of the n observations already seen
     */
    arma::mat V_X_inv;

public:
    /**
     * Constructor of a Pivot instance
     * @param X_init An initial batch of observations (d x n matrix, where d is the dimension and n the number of observations)
     */
    Pivot(arma::mat X_init);


    /**
     * Update the internal state by taking into account a new observation
     * @param X_new A new observation
     */
    void add(arma::vec X_new);

    /**
     * Update the internal state by removing an old observation
     * @param X_old An old observation
     */
    void remove(arma::vec X_old);

    /**
     * Compute and return the current pivot
     * @return s*_2
     */
    arma::vec s2star();

    /**
     * Return the mean of the observations
     * @return mean(X)
     */
    arma::mat mean();

    /**
     * Return the covariance of the observations
     * @return cov(X)
     */
    arma::mat cov();

    /**
     * Get the current number of observations taken into account
     * @return n
     */
    size_t n_obs();

    /**
     * Get the dimension of the observations
     * @return
     */
    size_t dim();


};


#endif //PIVOT_H
