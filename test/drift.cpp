#include "folding.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <random>


arma::mat binormal_with_drift(size_t N, double ratio = 0.5, double speed = 1e-3) {
    // random device
    std::random_device rd;
    std::default_random_engine gen(rd());
    std::uniform_real_distribution<double> u(0, 1);

    // fill the data
    arma::mat X(1, N, arma::fill::zeros);
    for (size_t i = 0; i < N; i++) {
        X.col(i) = arma::randn(1, 1); // N(0,1) -> first mode (fixed)
        if (u(gen) < ratio) {
            X.col(i) += speed * i; // N(speed*i , 1) -> second mode (drifting)
        }
    }

    return X;
}


int main(int argc, const char *argv[]) {

    arma::arma_rng::set_seed_random();

    size_t N = 100000;
    double ratio = 0.5;
    double speed = 1e-4;
    size_t win_size = 2000;

    arma::mat X = binormal_with_drift(N, ratio, speed);
    StreamFolding sf(win_size);

    double p;
    double Phi;
    bool u;
    auto mod = (size_t) (N / 20);

    const int wi = 9;
    const int wphi = 8;
    const int wp = 12;

    std::cout << std::right;
    std::cout << setw(wi) << "Iteration";
    std::cout << setw(wphi) << "Phi";
    std::cout << setw(wp) << "p-value" << std::endl;
    std::cout.fill('-');
    std::cout << setw(wi + wphi + wp) << '-' << std::endl;
    std::cout.fill(' ');
    for (size_t i = 0; i < N; i++) {
        sf.update(X.col(i));
        if ((i > 0) && (i % mod == 0)) {
            Phi = sf.folding_test(&u, &p);
            std::cout << std::fixed << std::setprecision(4);
            std::cout << setw(wi) << i;
            std::cout << setw(wphi) << Phi;
            std::cout << std::scientific << setw(wp) << p << std::endl;
        }
    }
    return 0;
}
