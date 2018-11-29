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

One great feature of the FTU is that it can be computed/updated incrementally over a sliding window. Then it is possible to monitor the *unimodality character* of streaming data over time.