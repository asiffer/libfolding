//
// Created by asr on 01/06/18.
//

#include "streamfolding.h"


double pval(double x, double e) {
    return pval_a * (1.0 - x) - pval_b * std::log(x) - e;
}


double bisection(pvalfun f, double param, double a, double b, double eps = 1e-12) {
    double a_ = a;
    double b_ = b;
    double m_;

    while ((b_ - a_) > eps) {
        m_ = .5 * (a_ + b_);
        if (f(a_, param) * f(m_, param) <= 0) {
            b_ = m_;
        } else {
            a_ = m_;
        }
    }

    return m_;
}


StreamFolding::StreamFolding(size_t depth) : depth_(depth), ring_(ArmaRing(depth)) {
    this->initialized_ = false;
    this->pivot_ = nullptr;
    this->iteration_ = 0;
    this->dimension_ = -1;
}

StreamFolding::~StreamFolding() {
    delete this->pivot_;
}


void StreamFolding::update(arma::vec v) {
    this->iteration_++;
    int ring_status = this->ring_.update(v);

    if (not this->initialized_) {
        if (this->iteration_ >= 10) {
            try {
                arma::mat X_init = this->ring_.dump();
                this->pivot_ = new Pivot(X_init);
                this->initialized_ = true;
                this->dimension_ = X_init.n_rows;
            }
            catch (const std::runtime_error &e) {
                cout << "[it " << this->iteration_ << "] Pivot initialization failed" << endl;
            }
        }
    } else {
        this->pivot_->add(v);
        if (ring_status == ArmaRing::STATUS::FILLED) {
            this->pivot_->remove(this->ring_.get_last_erased());
        }
    }

}

bool StreamFolding::is_initialized() {
    return this->initialized_;
}


arma::mat StreamFolding::mean() {
    return this->pivot_->mean();
}


arma::mat StreamFolding::cov() {
    return this->pivot_->cov();
}


arma::vec StreamFolding::s2star() {
    return this->pivot_->s2star();
}


double StreamFolding::folding_test(bool *unimodal, double *p_value) {
    double r = pow(1. + this->dimension_, 2);
    double Phi = r * this->ring_.folded_var(this->s2star()) / arma::trace(this->cov());
    *unimodal = (Phi >= 1.0);
    double e = std::abs(Phi - 1.0) * std::sqrt(1. * this->ring_.size()) / (std::log(1. * this->dimension_) + pval_c);
    *p_value = bisection(pval, e, 0.0, 1.0);
    return Phi;
}

arma::mat StreamFolding::dump() {
    return this->ring_.dump();
}