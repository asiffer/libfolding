//
// Created by asr on 31/05/18.
//

#include "pivot.h"

using namespace std;

arma::rowvec observation_squared_norm(arma::mat X) {
    return arma::sum(arma::square(X), 0);
}


arma::mat sherman_morrison(const arma::mat &Ainv, const arma::vec &u, const arma::vec &v) {
    arma::rowvec vt = v.t();
    arma::mat R = (vt * Ainv * u);
    double r = 1. + R[0];
    arma::mat B = (1. / r) * Ainv * u * vt * Ainv;
    return Ainv - B;
}


void inplace_sherman_morrison(arma::mat &Ainv, const arma::vec &u, const arma::vec &v) {
    arma::rowvec vt = v.t();
    arma::mat R = (vt * Ainv * u);
    double r = 1. + R[0];
    arma::mat B = (1. / r) * Ainv * u * vt * Ainv;
    Ainv -= B;
}


Pivot::Pivot(arma::mat Xinit) { // Xinit is d x n where d is the dimension
    int dimension = Xinit.n_rows;
    arma::mat V_X = Xinit * Xinit.t();
    this->n = 0;
    this->S_X = arma::vec(dimension, arma::fill::zeros);
    this->S_X2 = arma::mat(dimension, dimension, arma::fill::zeros);
    this->S_R = 0.0;
    this->V_XR = arma::vec(dimension, arma::fill::zeros);
    this->V_X_inv = arma::inv_sympd(V_X);

    double R_new;
    arma::vec X_new;

    for (unsigned int i = 0; i < Xinit.n_cols; i++) {
        //this->add(Xinit.col(i));
        X_new = Xinit.col(i);
        R_new = pow(arma::norm(X_new), 2);
        this->n++;
        this->S_X += X_new;
        this->S_X2 += X_new * X_new.t();
        this->S_R += R_new;
        this->V_XR += R_new * X_new;
    }

    /*this->S_X = arma::sum(Xinit, 1);
    this->S_R = (double)arma::sum(observation_squared_norm(Xinit));*/


}

void Pivot::add(arma::vec X_new) {
    double R_new = pow(arma::norm(X_new), 2);
    this->n++;
    this->S_X += X_new;
    this->S_X2 += X_new * X_new.t();
    this->S_R += R_new;
    this->V_XR += R_new * X_new;

    inplace_sherman_morrison(this->V_X_inv, X_new, X_new);

}

void Pivot::remove(arma::vec X_old) {
    double R_old = pow(arma::norm(X_old), 2);
    this->n--;
    this->S_X -= X_old;
    this->S_X2 -= X_old * X_old.t();
    this->S_R -= R_old;
    this->V_XR -= R_old * X_old;

    inplace_sherman_morrison(this->V_X_inv, -X_old, X_old);
}


arma::vec Pivot::s2star() {
    double r = 1.0 / n;
    arma::vec rS_X = r * this->S_X;
    arma::mat Sigma_inv = this->n * sherman_morrison(this->V_X_inv, -this->S_X, rS_X);
    arma::mat C = r * (this->V_XR - rS_X * this->S_R);
    return 0.5 * Sigma_inv * C;
}


arma::mat Pivot::mean() {
    return this->S_X / (1. * this->n);
}


arma::mat Pivot::cov() {
    double r = 1.0 / this->n;
    return (r * this->S_X2) - (r * this->S_X) * (r * this->S_X.t());
}


size_t Pivot::n_obs() {
    return this->n;
}

