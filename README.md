# libfolding

`libfolding` is a `C++` library which implements the **folding test of unimodality** (FTU) over streaming data.
This library is based on [Armadillo](http://arma.sourceforge.net/).

### Unimodality Testing

The aim of unimodality testing is to check if the distribution of the data is unimodal or multimodal.


| unimodal | multimodal |
| --- | --- |
|![unimodal](https://github.com/asiffer/libfolding/blob/master/img/1mode.png) | ![unimodal](https://github.com/asiffer/libfolding/blob/master/img/2modes.png) |


For many people it may refer to clustering but it is weaker (it does not clearly find neither the clusters 
nor the number of clusters) but it is generally more efficient (lower complexity)

Here is presented the univariate case, but the FTU works also on multivariate distributions!

### Manual installation

As usual, you just need to clone the repo
```commandline
git clone https://github.com/asiffer/libfolding.git
```
And then build and install it
```commandline
cd libfolding/
make
make install
```
During the installation, you can set the location of the library and its headers.
By default, there are put in `/usr/lib` and `/usr/include/libfolding`. You can change it with the variable `INSTALL_LIB_DIR` and
`INSTALL_HEAD_DIR`.

### Installation through debian package

The easiest way is to use the debian package. You just need to add the following pppa:
```commandline
sudo add-apt-repository ppa:asiffer/libfolding
sudo apt-get update
```
Then you can install the library
```commandline
sudo apt install libfolding
```
and the headers (if desired)
```commandline
sudo apt install libfolding-dev
```

### Examples



#### Multivariate normal random sample
```c++
#include "streamfolding.h"
#include <iostream>
#include <sstream>
#include <iomanip>

#define UNDERLINE "\033[4m"
#define END  "\x1B[0m"

int main(int argc, const char *argv[]) {
    arma::arma_rng::set_seed_random();

    const int N = 5000;
    const int d = 2;

    arma::mat Cov = 0.5 * arma::ones(d, d) + 0.5 * arma::eye(d, d); // covariance matrix
    arma::vec Mean(d, arma::fill::zeros); // mean vector
    arma::mat Xinit = arma::mvnrnd(Mean, Cov, N); // size d x N

    const size_t window_size = 2000;
    StreamFolding sf(window_size); // initialization

    std::cout << std::fixed << std::setprecision(2);
    std::cout << endl << UNDERLINE << "Testing on a multivariate normal distribution" << END << std::endl;
    std::cout << "Dimension d = " << d << ", #pts = " << N << std::endl;
    Mean.print("Mean");
    Cov.print("Covariance matrix");

    // Add new data (update the object)
    for (size_t i = 0; i < N; i++) {
        sf.update(Xinit.col(i));
    }
    
    // Folding test
    bool uni;
    double pvalue;
    cout << "Folding Statistics : " << sf.folding_test(&uni, &pvalue) << endl;
    cout << std::scientific;
    stringstream ss;
    ss << "Unimodal ? " << (uni ? "Yes" : "No") << " (p-value: " << pvalue << ")";
    std::cout << setw(40) << ss.str() << std::endl;
}
```

To compile this program you have to link with both libraries (`libfolding` and `libarmadillo`)
```commandline
g++ -std=c++11 -Wall -pedantic -I/usr/include/libfolding main.cpp -o example -lfolding -larmadillo 
```

Finally, if you run this example, you can check that the test outputs well that the distribution is unimodal.
```shell
./example
Testing on a multivariate normal distribution
Dimension d = 2, #pts = 5000
Mean
        0
        0
Covariance matrix
   1.0000   0.5000
   0.5000   1.0000
Folding Statistics : 2.94
   Unimodal ? Yes (p-value: 9.09495e-13)

```



#### Bi-normal univariate random sample

```c++
#include "streamfolding.h"
#include <iostream>
#include <sstream>
#include <iomanip>

#define UNDERLINE "\033[4m"
#define END  "\x1B[0m"

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
    
    std::cout << std::fixed << std::setprecision(2);
    std::cout << endl << UNDERLINE << "Testing on a bi-normal distribution" << END << std::endl;
    std::cout << "Mode 1 : µ = " << M1 << ", σ = 1.0, #pts = " << N1 << " (" << (100. * N1 / N) << "%)" << std::endl;
    std::cout << "Mode 2 : µ = " << M2 << ", σ = 1.0, #pts = " << N2 << " (" << (100. * N2 / N) << "%)" << std::endl;
    
    arma::rowvec X = arma::shuffle(Xinit).t();
    
    const size_t window_size = 5000;
    StreamFolding sf(window_size);
    
    
    for (size_t i = 0; i < N; i++) {
        sf.update(X.col(i));
    }
    
    // Folding test
    bool uni;
    double pvalue;
    std::cout << "Folding Statistics : " << sf.folding_test(&uni, &pvalue) << std::endl;
    std::cout << std::scientific;
    stringstream ss;
    ss << "Unimodal ? " << (uni ? "Yes" : "No") << " (p-value: " << pvalue << ")";
    std::cout << setw(40) << ss.str() << std::endl;
}
```

With the same compilation steps as before, it may output that the distribution is multimodal
```shell
./example
Testing on a bi-normal distribution
Mode 1 : µ = 0.00, σ = 1.0, #pts = 2000 (20.00%)
Mode 2 : µ = 5.00, σ = 1.0, #pts = 8000 (80.00%)
Folding Statistics : 0.82
    Unimodal ? No (p-value: 9.09495e-13)

```
