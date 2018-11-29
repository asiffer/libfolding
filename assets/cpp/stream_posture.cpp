#include "folding.h"
#include <iostream>

void help() {
    std::cout << "stream_posture [WINDOW_SIZE] [FTU_COMPUTE_PERIOD]" << std::endl << std::endl;
    std::cout << "Arguments\n---------" << std::endl;
    std::cout << "\tWINDOW_SIZE       " << '\t' << "Number of data to compute the FTU (default: 600)" << std::endl;
    std::cout << "\tFTU_COMPUTE_PERIOD" << '\t' << "Computing frequency of FTU (default: 50)" << std::endl << std::endl;
}
    

int main(int argc, const char * argv[]) {
    // stream parameters
    size_t window_size = 600;
    size_t ftu_compute_period = 50; 

    if (argc > 1) {
        window_size = (size_t)std::atol(argv[1]);
        if (window_size == 0) {
            help();
            throw std::runtime_error("Incorrect window size");
        }
        if (argc > 2) {
            ftu_compute_period = (size_t)std::atol(argv[2]);
            if (ftu_compute_period == 0) {
                help();
                throw std::runtime_error("Incorrect computing period");
            }
        }
    }
    
    // loading data
    arma::mat X;
    X.load("../data/data_A01.csv",arma::csv_ascii); // load 'posture' data
    arma::inplace_strans(X); // inplace transposition of X (to have columns -> observations)
    

    // FTU output
    bool unimodal;
    double p_value;
    double folding_statistics;
    
    // Main object
    StreamFolding sf(window_size);
    
    // loop over the data
    for (size_t i = 0; i<(size_t)X.n_cols; i++) {
        sf.update(X.col(i)); // take into account this new value
        // compute/print results online
        if ( (i > window_size) && (i % ftu_compute_period == 0) ) {
            folding_statistics = sf.folding_test(&unimodal, &p_value);
            std::cout << i << '\t' << folding_statistics << std::endl;
        }
    }
    return 0;
}
