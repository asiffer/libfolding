//
// Created by asr on 01/06/18.
//

#include "folding.h"


/* TIMING FUNCTION */
long int duration(T start, T end) { // in microsec
    return std::chrono::duration_cast<usec>(end - start).count();
}

long int operator-(const T &end, const T &start) {
    return (duration(start, end));
}


BatchFolding::BatchFolding(arma::mat X) {

    this->n = (size_t) X.n_cols;
    this->d = (size_t) X.n_rows;

    T _start = std::chrono::steady_clock::now();

    arma::rowvec X_square_norm = arma::sum(arma::square(X), 0);  // |X|²
    arma::mat mat_cov = arma::cov(X.t());
    double trace = arma::trace(mat_cov);  // Tr(cov(X))

    arma::vec cov_norm = arma::cov(X.t(), X_square_norm.t());  // cov(X,|X|²)
    arma::vec pivot = 0.5 * arma::solve(mat_cov, cov_norm);  // 0.5 * cov(X)^{-1} * cov(X,|X|²)
    arma::rowvec X_reduced = arma::sqrt(arma::sum(arma::square(X.each_col() - pivot), 0));  // |X-s*|
    this->Phi = pow(1. + this->d, 2) * arma::var(X_reduced) / trace;
    this->p_value = 1. - significance(Phi, this->n, this->d);

    T _end = std::chrono::steady_clock::now();

    this->unimodal = (this->Phi >= 1.0);
    this->s2star = pivot;
    this->usec = _end - _start;
}


void BatchFolding::print() {
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Folding Test of Unimodality" << std::endl;
    std::cout << "Number of observations: " << this->n << " (dim: " << this->d << ")" << std::endl;
    auto itsec = (size_t) ((1e6 * this->n) / ((double) this->usec));
    std::cout << "Computation time: " << this->usec << " µs (~ " << itsec << " it/s)" << std::endl;
    std::cout << "Folding Statistics: " << this->Phi << std::endl;

    cout << std::scientific;
    std::cout << '\t' << "The distribution is then " << (this->unimodal ? "unimodal" : "multimodal")
              << " (with p-value: " << this->p_value << ")" << std::endl;

    this->s2star.print("Folding pivot");
}


