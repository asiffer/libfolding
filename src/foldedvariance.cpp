//
// Created by asr on 12/07/18.
//

#include "foldedvariance.h"


FoldedVariance::FoldedVariance(arma::mat *M, arma::vec &s_star, double *sum, double *sum2) :
        data_(M), s_(s_star), sum_(sum), sum2_(sum2) {
}


void FoldedVariance::operator()(const tbb::blocked_range<size_t> &r) const {
    double norm;
    for (size_t i = r.begin(); i != r.end(); ++i) {
        norm = arma::norm(this->data_->col(i) - this->s_);
        *this->sum_ += norm;
        *this->sum2_ += norm * norm;
    }
}