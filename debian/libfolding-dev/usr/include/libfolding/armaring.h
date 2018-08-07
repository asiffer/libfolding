//
// Created by asr on 01/06/18.
//

#ifndef ARMARING_H
#define ARMARING_H

#include <armadillo>

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
    * Return the maximum size of the container
    * @return
    */
    size_t max_size();

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


#endif // ARMARING_H
