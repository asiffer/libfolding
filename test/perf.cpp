//
// Created by asr on 06/06/18.
//

#include "folding.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>

using namespace std;


using T = std::chrono::steady_clock::time_point;
using usec = std::chrono::microseconds;
using sec = std::chrono::seconds;
//std::chrono::_V2::steady_clock::time_point (*tick)() = std::chrono::steady_clock::now;

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


/* TIMING FUNCTION */
long int duration(T start, T end) {
    return std::chrono::duration_cast<usec>(end - start).count();
}

long int operator-(const T &end, const T &start) {
    return (duration(start, end));
}


int main(int argc, const char *argv[]) {

    arma::arma_rng::set_seed_random();

    const size_t N = 1000000;
    const size_t win_size = 1000;
    const size_t max_dim = 3;

    const int w = 15;
    cout << left << "Window size: " << win_size << endl;
    cout << setw(w) << "Dimension";
    cout << setw(w) << "#data";
    cout << setw(w) << "Time (µs)";
    cout << setw(w) << "Speed (it/s)" << endl;
    for (size_t d = 1; d <= max_dim; d++) {
        arma::mat Xinit = arma::randu(d, N); // size d x N
        StreamFolding sf(win_size);
        T start_ = std::chrono::steady_clock::now();
        for (size_t i = 0; i < N; i++) {
            sf.update(Xinit.col(i));
        }
        T end_ = std::chrono::steady_clock::now();

        cout << setw(w) << d;
        cout << setw(w) << N;
        cout << setw(w) << end_-start_;
        cout << setw(w) << (size_t)(1.0e6*N/(end_-start_)) << endl;
    }


}

