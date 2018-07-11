# libfolding

`libfolding` is a C++ library which implements the **folding test of unimodality** (FTU) over streaming data

### Unimodality Testing

The aim of unimodality testing is to check if the distribution of the data is unimodal or multimodal.


| unimodal | multimodal |
| --- | --- |
|![unimodal](https://github.com/asiffer/libfolding/blob/master/img/1mode.png) | ![unimodal](https://github.com/asiffer/libfolding/blob/master/img/2modes.png) |


For many people it may refer to clustering but it is weaker (it does not clearly find neither the clusters 
nor the number of clusters) but it is generally more efficient (lower complexity)

Here is presented the univariate case, but the FTU works also on multivariate distributions!

### The folding test of unimodality
