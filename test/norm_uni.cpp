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

    const int N = 20000;
    const int d = 1;

    arma::mat Cov = 0.5 * arma::ones(d, d) + 0.5 * arma::eye(d, d);
    arma::vec Mean(d, arma::fill::ones);
    arma::mat Xinit = arma::mvnrnd(Mean, Cov, N); // size d x N

    StreamFolding sf(10000);

    cout << std::fixed << std::setprecision(2);
    cout << endl << UNDERLINE << "Testing on a univariate normal distribution" << END << endl;
    cout << "Dimension d = " << d << ", #pts = " << N << endl;
    Mean.print("Mean");
    Cov.print("Variance");


    for (size_t i = 0; i < N; i++) {
        sf.update(Xinit.col(i));
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
    //cout << "size of D : " << D.n_rows << " x " << D.n_cols << endl;
    arma::mat cov_batch = arma::cov(D.t());
    //cout << "size of batch cov : " << cov_batch.n_rows << " x " << cov_batch.n_cols << endl;
    arma::mat cov_stream = sf.cov();

    //cov_batch.print("Batch cov :");

    double cov_delta = arma::norm(cov_batch - cov_stream);
    //cout << std::scientific << cov_delta << endl;
    if (cov_delta < 1e-3) {
        cout << OK << endl;
    } else {
        cout << FAIL << endl;
    }


    cout << setw(40) << "Checking the pivot computation ... ";
    arma::vec s2_star = sf.s2star();
    double s2star_delta = arma::norm(s2_star - Mean);
    //cout << s2star_delta << endl;
    if (s2star_delta < 8e-2) {
        cout << OK << endl;
    } else if (s2star_delta < 1e-1) {
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

    if (uni) {
        cout << OK << endl;
    } else {
        cout << FAIL << endl;
    }
    cout << endl;

}
