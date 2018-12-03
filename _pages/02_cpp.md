---
layout: default
title: C++
permalink: /cpp
---

### Contents

- <a href="#introduction">Introduction</a>
- <a href="#installation">Installation</a>
   - <a href="#manual_installation">Manual installation</a>
   - <a href="#debian">Installation through debian package</a>
- <a href="#getting_started">Getting started</a>
   - <a href="#batch_example">Batch example</a>
   - <a href="#streaming_example">Streaming example</a>

## Introduction
<a name="introduction"></a> 

`libfolding` is initially a `C++` library which implements the folding test of unimodality (FTU). It is designed to work on streaming data but you can also perform the test on a batch dataset. This library is based on [Armadillo](http://arma.sourceforge.net/) for linear algebra.

## Installation
<a name="installation"></a> 

### Manual installation
<a name="manual_installation"></a> 

As usual, you just have to clone the repo
```bash
git clone https://github.com/asiffer/libfolding.git
```
And then build and install it
```bash
cd libfolding/
make
make install
```
During the installation, you can set the location of the library and its headers.
By default, there are put in `/usr/lib` and `/usr/include/libfolding`. You can change it with the variable `INSTALL_LIB_DIR` and
`INSTALL_HEAD_DIR`.

### Installation through debian package
<a name="debian"></a> 

The easiest way is to use the debian package. You just need to add the following ppa:
```bash
sudo add-apt-repository ppa:asiffer/libfolding
sudo apt-get update
```
Then you can install the library
```bash
sudo apt install libfolding
```
and the headers (if desired)
```bash
sudo apt install libfolding-dev
```

## Getting started
<a name="getting_started"></a> 

### Batch example
<a name="batch_example"></a> 

The first thing you want to do is probably to perform the **FTU** on a dataset. As `libfolding` is built upon `libarmadillo` (for linear algebra), you need to use its objects to load your data. An example is given below.

```c++
// file batch_iris.cpp
#include "folding.h"

int main(int argc, const char* argv[]) {
    arma::mat X; // declare the matrix object
    // load the 'iris' dataset
    X.load("../data/iris.csv",arma::csv_ascii); 
    X.shed_col(4); // remove the species columns
    BatchFolding bf(X.t()); // perform the FTU
    bf.print(); // print results
    return 0;
}
```

So, you need to use the `arma::mat` object provided by `libarmadillo` to import and manipulate data. Then, you may notice that the FTU is performed on the transposed matrix. Actually, as Armadillo uses column-major ordering convention, we do consider a column as an observation. On linux system you might compile as follows:

```bash
g++ -std=c++11 -Wall -pedantic -I/usr/include/libfolding -o ./batch_iris  ./batch_iris.cpp -lfolding -larmadillo
```

Finally, by running `./batch_iris`, the output may look like this:
```bash
Folding Test of Unimodality
Number of observations: 151 (dim: 4)
Computation time: 261 µs (~ 578544 it/s)
Folding Statistics: 3.414
        The distribution is then unimodal (with p-value: 0.000e+00)
Folding pivot
   4.4659
   2.9085
   3.6206
   1.8279
```

In a nutshell, the test claims that the distribution is rather unimodal. Details about the output of the FTU are given <a href="{{ site.baseurl }}/unimodality-testing#output">here</a>.

The `BatchFolding` object does not offer more methods but you can access to the data printed above.

```cpp
bool unimodal; // the FTU result
double Phi; // the folding statistics
double p_value; // the p-value of the test (lower is better)
arma::vec s2star; // the folding pivot
long int usec; // the time to perform the test (~ debug information)
```


### Streaming example
<a name="streaming_example"></a> 

One great feature of the FTU is that it can be computed/updated incrementally over a sliding window. Then it is possible to monitor over time the *unimodality character* of streaming data.

The following example in more theoretical and deals about a drifting mode.
Data are initially under two modes which are initially at the same position (0).
Then a mode drifts to the right while the other one remains the same. In this example, data are initially 
unimodal and become multimodal.

<div class="figure" style="float: left; width: 100%; margin: 1em auto;">
   <object type="image/svg+xml" data="assets/drift.svg"></object>
</div>

In this example, we want to perform the FTU several times on this stream to check if we monitor the modality change (unimodality -> multimodality).

For the next part we will use the following headers.
```cpp
#include "folding.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <random>
```

The generation of the data is given by the code below.

```cpp
/**
 * @brief   Generates N streaming points X_1, ... X_n such that 
 *          X_i is drawn from (1-ratio)*N(0,1) + ratio*N(i*speed, 1)
 * @param   N     The number of data to generate
 * @param   ratio The proportion of data inside the drifiting mode 
 *                (0.5 means that the modes are balanced)
 * @param   speed The drifting speed
 * @returns A 1xN armadillo matrix
*/
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
```

The object `StreamFolding` is designed to carry out the folding test of unimodality within a sliding window.
At any time we can do the test and check how the data behave in this time slot.


```cpp
int main(int argc, const char *argv[]) {

   arma::arma_rng::set_seed_random();

   // data parameters
   size_t N = 100000;
   double ratio = 0.5;
   double speed = 1e-4;

   // window size
   size_t win_size = 2000;

   // data
   arma::mat X = binormal_with_drift(N, ratio, speed);

   // initialization
   StreamFolding sf(win_size);

   // temp or side variables
   double p;
   double Phi;
   bool u;
   auto mod = (size_t) (N / 50);

   const int wi = 9;
   const int wphi = 8;
   const int wp = 12;

   // Printing the results header
   std::cout << std::right;
   std::cout << setw(wi) << "Iteration";
   std::cout << setw(wphi) << "Phi";
   std::cout << setw(wp) << "p-value" << std::endl;
   std::cout.fill('-');
   std::cout << setw(wi + wphi + wp) << '-' << std::endl;
   std::cout.fill(' ');

   // loop over the observations
   for (size_t i = 0; i < N; i++) {
      sf.update(X.col(i));

      // breakpoint to perform the test
      if ((i > 0) && (i % mod == 0)) {
         // compute the test!
         Phi = sf.folding_test(&u, &p);

         // printing stuff
         std::cout << std::fixed << std::setprecision(4);
         std::cout << setw(wi) << i;
         std::cout << setw(wphi) << Phi;
         std::cout << std::scientific << setw(wp) << p << std::endl;
      }
   }
   return 0;
}
```

We compile it as before. The execution result may look like this:
```bash
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

Obviously, the folding statistics (`Phi`) decreases over time (unimodal -> multimodal). 

The modality change occurs around iteration 30000: as the speed is set to $10^{-4}$, it means that the second mode is centered at $x=3$ at this time. Actually, this moment visually corresponds to the modes split.

Close to 1 the output of the test is naturally less significant (higher p-value).