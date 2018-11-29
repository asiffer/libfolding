#include "folding.h"

int main(int argc, const char* argv[]) {
    arma::mat X; // declare the matrix object
    X.load("../data/iris.csv",arma::csv_ascii); // load 'iris' data
    X.shed_col(4); // remove the species columns
    BatchFolding bf(X.t()); // perform the FTU
    bf.print(); // print results
    return 0;
}
