//
// Created by asr on 01/06/18.
//

#include "armaring.h"


ArmaRing::ArmaRing(size_t max_size) : max_size_(max_size) {
    this->cursor_ = 0;
    this->memory_ = arma::mat();
    this->last_erased_ = arma::vec();
}

int ArmaRing::update(arma::vec v) {

    if (this->max_size_ == 0) { // infinite
        this->memory_.insert_cols(this->cursor_, v);
        this->cursor_++;
        return ArmaRing::STATUS::INFINITE;


    } else if (this->memory_.n_cols < this->max_size_) { // not filled
        this->memory_.insert_cols(this->cursor_, v);
        this->cursor_ = (this->cursor_ + 1) % this->max_size_;
        return ArmaRing::STATUS::UNFILLED;


    } else if (this->memory_.n_cols == this->max_size_) { // cruising regime
        this->last_erased_ = this->memory_.col(this->cursor_);
        this->memory_.col(this->cursor_) = v;
        this->cursor_ = (this->cursor_ + 1) % this->max_size_;
        return ArmaRing::STATUS::FILLED;

    }
    return ArmaRing::STATUS::ERROR;
}

size_t ArmaRing::size() {
    return this->memory_.n_cols;
}


double ArmaRing::folded_var(arma::vec s2_star) {
    size_t n = this->size();
    double sum = 0.0;
    double sum2 = 0.0;
    FoldedVariance fv(&this->memory_, s2_star, &sum, &sum2);
    tbb::parallel_for(tbb::blocked_range<size_t>(0, n), fv);
    return sum2 / n - pow(sum / n, 2.0);
}


arma::mat ArmaRing::dump() {
    return this->memory_;
}


arma::vec ArmaRing::get_last_erased() {
    return this->last_erased_;
}


FoldedVariance::FoldedVariance(arma::mat *M, arma::vec &s_star, double *sum, double *sum2) :
        data_(M), s_(s_star), sum_(sum), sum2_(sum2) {
}


void FoldedVariance::operator()(const tbb::blocked_range <size_t> &r) const {
    double norm;
    for (size_t i = r.begin(); i != r.end(); ++i) {
        norm = arma::norm(this->data_->col(i) - this->s_);
        *this->sum_ += norm;
        *this->sum2_ += norm * norm;
    }
}

