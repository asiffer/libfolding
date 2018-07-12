/**
	@file interface.cpp
	@brief C interface (for python notably)
	@author Alban Siffer
	@date 05 July 2018
*/

#include "folding.h"

using namespace std;

extern "C" {

StreamFolding *sf_new(size_t depth) {
    return new StreamFolding(depth);
}

void sf_update(StreamFolding *sf, double *array_ptr, size_t n_elem, bool copy, bool strict) {
    sf->update(arma::vec(array_ptr, n_elem, copy, strict));
}

bool sf_is_initialized(StreamFolding *sf) {
    return sf->is_initialized();
}

void sf_folding_test(StreamFolding *sf, bool *unimodal, double *p_value, double *Phi) {
    *Phi = sf->folding_test(unimodal, p_value);
}


void sf_mean(StreamFolding *sf, double *output) {
    arma::vec m = sf->mean();
    for (unsigned int i = 0; i < m.size(); i++) {
        output[i] = m[i];
    }
}


void sf_cov(StreamFolding *sf, double *output) {
    arma::mat c = sf->cov();
    size_t d = c.n_rows;
    size_t k = 0;
    for (size_t i = 0; i < d; i++) {
        for (size_t j = 0; j < d; j++) {
            output[k] = c(i, j);
            k++;
        }
    }
}


void sf_s2star(StreamFolding *sf, double *output) {
    arma::vec s = sf->s2star();
    for (unsigned int i = 0; i < s.size(); i++) {
        output[i] = s[i];
    }
}


void sf_dump(StreamFolding *sf, double *output) {
    arma::mat X = sf->dump();
    size_t n = X.n_rows;
    size_t p = X.n_cols;
    size_t k = 0;
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < p; j++) {
            output[k] = X(i, j);
            k++;
        }
    }
}


void
batch_folding_test(double *array_ptr, size_t n, size_t p, bool *unimodal, double *pval, double *Phi, long int *usec) {
    arma::mat M(array_ptr, n, p, false);
    BatchFolding bf(M);
    *unimodal = bf.unimodal;
    *pval = bf.p_value;
    *Phi = bf.Phi;
    *usec = bf.usec;
}

}

