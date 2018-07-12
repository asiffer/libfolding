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


size_t ArmaRing::max_size() {
    return this->max_size_;
}



double ArmaRing::folded_var(arma::vec s2_star) {
    arma::rowvec X_reduced = arma::sqrt(arma::sum(arma::square(this->memory_.each_col() - s2_star), 0));  // |X-s*|
    return arma::var(X_reduced);
}


arma::mat ArmaRing::dump() {
    return this->memory_;
}


arma::vec ArmaRing::get_last_erased() {
    return this->last_erased_;
}




