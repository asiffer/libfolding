#include "streamfolding.h"
#include <iostream>

void inplace_sherman_morrison(arma::mat &Ainv, const arma::vec &u, const arma::vec &v);

int main(int argc, const char *argv[]) {

    arma::arma_rng::set_seed_random();
    /*arma::mat A(3, 3, arma::fill::randn);
    const arma::vec u(3, arma::fill::randu);
    const arma::vec v(3, arma::fill::randu);

    A.print();
    inplace_sherman_morrison(A, u, v);
    A.print();*/

    const int N = 5000;
    const int d = 2;

    arma::mat Cov = 0.5 * arma::ones(d, d) + 0.5 * arma::eye(d, d);
    Cov.print("Cov : ");
    arma::vec Mean(d, arma::fill::zeros);
    Mean.print("Mean : ");

    arma::mat Xinit = arma::mvnrnd(Mean, Cov, N); // size d x N

    StreamFolding sf(2000);


    for (size_t i = 0; i < N; i++) {
        sf.update(Xinit.col(i));
    }

    sf.cov().print("Covariance : ");
    sf.s2star().print("s2* :");

    cout << "folding test : " << sf.folding_test() << endl;


    //arma::mat V_X_inv_init = Xinit * Xinit.t();

    //V_X_inv_init.print();
    /*Pivot P(Xinit);



    cout << P.n_obs() << endl;
    arma::vec s2 = P.s2star();
    s2.print("s2 (initial) : ");

    P.cov().print("Empirical Covariance : ");*/

    /*Matrix<3, 4, double> M;
    M(1, 2) = 5.0;
    M(2, 1) = 3.0;
    M(3, 2) = 4.0;
    M.print();

    Matrix<3, 4, double> K = M * 3;
    K.print();
    M.print();*/

    //Matrix<4,3, double> N;

    //Matrix<3, 3, double> C = M*N;
    //C.print();
    /*M.print();
    cout << "M_12: " << M(1,2) << endl;
    cout << "M_21: " << M(2,1) << endl;

    auto Mt = M.tr();
    cout << "Mt_12: " << Mt(1,2) << endl;
    cout << "Mt_21: " << Mt(2,1) << endl;*/

}