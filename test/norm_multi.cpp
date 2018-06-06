#include "streamfolding.h"
#include <iostream>
#include <sstream>
#include <iomanip>

/* COLORS FOR FANCY PRINT */
#define END  "\x1B[0m"
#define RED  "\x1B[31m"
#define GRN  "\x1B[32m"
#define YEL  "\x1B[33m"

#define OK "[" GRN "OK" END "]"
#define WARNING "[" YEL "WARNING" END "]"
#define FAIL "[" RED "FAIL" END "]"

#define UNDERLINE "\033[4m"

/* END OF COLORS FOR FANCY PRINT */



int main(int argc, const char *argv[]) {

    arma::arma_rng::set_seed_random();

    const int d = 1;

    const size_t N1 = 2000;
    const size_t N2 = 8000;
    size_t N = N1 + N2;

    const double M1 = 0.0;
    const double M2 = 5.0;

    arma::vec X1 = arma::randn(N1, d) + M1;
    arma::vec X2 = arma::randn(N2, d) + M2;

    arma::vec Xinit = arma::join_cols(X1, X2); /// size d x (N1+N2)


    cout << std::fixed << std::setprecision(2);
    cout << endl << UNDERLINE << "Testing on a bi-normal distribution" << END << endl;
    cout << "Mode 1 : µ = " << M1 << ", σ = 1.0, #pts = " << N1 << " (" << (100. * N1 / N) << "%)" << endl;
    cout << "Mode 2 : µ = " << M2 << ", σ = 1.0, #pts = " << N2 << " (" << (100. * N2 / N) << "%)" << endl;
    //cout << "Size of Xinit : " << Xinit.n_rows << " x " << Xinit.n_cols << endl;

    arma::rowvec X = arma::shuffle(Xinit).t();

    StreamFolding sf(2000);


    for (size_t i = 0; i < N; i++) {
        sf.update(X.col(i));
    }

    arma::mat D = sf.dump();


    cout << std::left;
    cout << setw(40) << "Checking the mean computation ... ";
    arma::vec mean_stream = sf.mean();
    arma::vec mean_batch = arma::mean(D, 1);
    double mean_delta = arma::norm(mean_batch - mean_stream);

    if (mean_delta < 1e-6) {
        cout << OK << endl;
    } else {
        cout << FAIL << endl;
    }

    cout << setw(40) << "Checking the variance computation ... ";
    arma::mat cov_batch = arma::cov(D, 1);
    arma::mat cov_stream = sf.cov();
    double cov_delta = arma::norm(cov_batch - cov_stream);

    if (cov_delta < 1e-6) {
        cout << OK << endl;
    } else {
        cout << FAIL << endl;
    }


    cout << setw(40) << "Checking the pivot computation ... ";
    arma::vec s2_star = sf.s2star();
    double s2star_err = arma::norm(s2_star - 0.5 * (M1 + M2)) / std::abs(0.5 * (M1 + M2));
    //cout << s2star_err << endl;
    if (s2star_err < 0.11) {
        cout << OK << endl;
    } else if (s2star_err < 0.15) {
        cout << WARNING << endl;
    } else {
        cout << FAIL << endl;
    }


    bool uni;
    double pvalue;
    cout << "Folding Statistics : " << sf.folding_test(&uni, &pvalue) << endl;
    cout << std::scientific;
    stringstream ss;
    ss << "Unimodal ? " << (uni ? "Yes" : "No") << " (p-value: " << pvalue << ")";
    cout << setw(40) << ss.str();

    if (not uni) {
        cout << OK << endl;
    } else {
        cout << FAIL << endl;
    }

    cout << endl;
}