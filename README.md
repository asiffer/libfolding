# libfolding

### Introduction
`libfolding` is a `C++` library which implements the **folding test of unimodality** (FTU). 
It is designed to work on streaming data but you can also perform the test on a batch dataset.
This library is based on [Armadillo](http://arma.sourceforge.net/).

### Unimodality Testing

The aim of unimodality testing is to check if the distribution of the data is unimodal or multimodal.


| unimodal | multimodal |
| --- | --- |
|![unimodal](https://github.com/asiffer/libfolding/blob/master/img/1mode.png) | ![unimodal](https://github.com/asiffer/libfolding/blob/master/img/2modes.png) |


For many people it may refer to clustering but it is weaker (it does not clearly find neither the clusters 
nor the number of clusters) but it is generally more efficient (lower complexity)

Here is presented the univariate case, but the FTU works also on multivariate distributions!

### Installation
#### Manual installation

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

#### Installation through debian package

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

#### Univariate normal random sample (batch)

In this example, we perform the folding test of unimodality 
on a centred gaussian sample with standard deviation 5.

```c++
#include "folding.h"

int main(int argc, const char * argv[]) {
    arma::arma_rng::set_seed_random();

    size_t N = 10000;
    double mean = 0.0;
    double std = 5.0;
    arma::rowvec X = std * arma::randn(1, N) + mean;
    
    BatchFolding bf(X);
    bf.print();
    return 0;
}
```

To compile this program you have to link with both libraries (`libfolding` and `libarmadillo`)
```commandline
g++ -std=c++11 -Wall -pedantic -I/usr/include/libfolding main.cpp -o example -lfolding -larmadillo 
```

Finally, if you run this example, you can check that the test outputs well that the distribution is unimodal.
```shell
$ ./example
Folding Test of Unimodality
Number of observations: 10000 (dim: 1)
Computation time: 1821 µs (~ 5491488 it/s)
Folding Statistics: 1.444
	The distribution is then unimodal (with p-value: 4.370e-02)
Folding pivot
  -0.1214
```

#### Drifting mode

Here we consider a streaming example. Data are under two modes which are initially at the same position (0).
Then a mode drifts to the right while the other one remains the same. In this example, data are initially 
unimodal and become multimodal.

The object `StreamFolding` is designed to carry out the folding test of unimodality within a sliding window.
At any time we can do the test and check how the data behave.


```c++
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
    auto mod = (size_t) (N / 50);

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
```

We compile it as before. The result may look like this:
```commandline
Iteration     Phi     p-value
-----------------------------
     5000  1.4418  0.0000e+00
    10000  1.4753  0.0000e+00
    15000  1.3866  0.0000e+00
    20000  1.3149  9.0961e-13
    25000  1.1633  1.5735e-06
    30000  1.0396  3.3863e-01
    35000  0.9137  9.2496e-03
    40000  0.7511  9.7316e-11
    45000  0.6506  9.0961e-13
    50000  0.5474  0.0000e+00
    55000  0.4672  0.0000e+00
    60000  0.3944  0.0000e+00
    65000  0.3341  0.0000e+00
    70000  0.3111  0.0000e+00
    75000  0.2662  0.0000e+00
    80000  0.2470  0.0000e+00
    85000  0.2263  0.0000e+00
    90000  0.1865  0.0000e+00
    95000  0.1765  0.0000e+00
```

Obviously, the folding statistics (`Phi`) decreases over time (unimodal -> multimodal). Close to 1 the output 
of the test is naturally less significant (higher p-value).


#### Multivariate normal random sample

As claimed, you can also carry out the test on multivariate data. The following example
analyze a sample drawn from a multivariate normal distribution.

```c++
#include "folding.h"

int main(int argc, const char * argv[]) {
    arma::arma_rng::set_seed_random();

    size_t N = 100000;
    size_t d = 3;
    arma::vec mean(d, arma::fill::zeros);
    arma::mat cov = 0.5 * arma::ones(d, d) + 0.5 * arma::eye(d, d);
    arma::mat X = arma::mvnrnd(mean, vov, N); // size d x N
    
    BatchFolding bf(X);
    bf.print();
    return 0;
}
```

The output may be the following (the folding statistics should be close to 3.0):

```commandline
$ ./example
Folding Test of Unimodality
Number of observations: 100000 (dim: 3)
Computation time: 26290 µs (~ 3803727 it/s)
Folding Statistics: 3.058
	The distribution is then unimodal (with p-value: 0.000e+00)
Folding pivot
  -0.0045
   0.0059
  -0.0081

```




