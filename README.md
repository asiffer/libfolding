# libfolding

`libfolding` is a `C++` library which implements the **folding test of unimodality** (FTU) over streaming data

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


