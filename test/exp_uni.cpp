#include "folding.h"
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
    stringstream ss;
    arma::arma_rng::set_seed_random();

    const int N = 50000;
    const int d = 1;


    double lambda = 0.5;
    arma::mat X = arma::randg(1, N, arma::distr_param(1.,1./lambda));

    //StreamFolding sf(10000);

    std::cout << std::fixed << std::setprecision(3) << std::left;
    std::cout << std::endl << UNDERLINE << "Testing on a univariate exponential distribution (batch)" << END << std::endl;
    std::cout << "Dimension d = " << d << ", #pts = " << N << std::endl;
    std::cout << "Parameter lambda = " << lambda << std::endl;

    BatchFolding bf(X);

    ss << "Pivot: " << bf.s2star.at(0);
    double s2star_delta = arma::norm(bf.s2star - 2./lambda);
    //std::cout << s2star_delta << std::endl;
    std::cout << setw(40) << ss.str();
    if (s2star_delta < 8e-2) {
        std::cout << OK;
    } else if (s2star_delta < 1e-1) {
        std::cout << WARNING;
    } else {
        std::cout << FAIL;
    }
    std::cout << std::endl;
    ss.str("");


    ss << "Folding Statistics : " << bf.Phi;
    std::cout << setw(40) << ss.str();
    double err = std::abs(bf.Phi - 1.54);
    if (err < 1e-1) {
        std::cout << OK;
    } else if (err < 2e-1) {
        std::cout << WARNING;
    } else {
        std::cout << FAIL;
    }
    std::cout << std::endl;
    ss.str("");
    
    std::cout << std::scientific;
    ss << "Unimodal ? " << (bf.unimodal ? "Yes" : "No") << " (p-value: " << bf.p_value << ")";
    std::cout << setw(40) << ss.str();
    if (bf.unimodal) {
        std::cout << OK;
    } else {
        std::cout << FAIL;
    }
    std::cout << std::endl;
    ss.str("");
}
