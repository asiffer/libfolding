---
layout: default
title: Unimodality Testing
permalink: /unimodality-testing
---

### Contents

- <a href="#introduction">Introduction</a>
- <a href="#mechanism">Understanding the folding mechanism</a>
- <a href="#output">Output of the test</a>
   - <a href="#phi">The folding statistics</a>
   - <a href="#p-value">The p-value</a>
   - <a href="#pivot">The folding pivot</a>
- <a href="#other">Other unimodality tests</a>

## Introduction
<a name="introduction"></a> 

The aim of unimodality testing is to check whether the distribution of the data is **unimodal** or **multimodal**.

For many people it may refer to clustering but it is weaker: it does not clearly find neither the clusters nor the number of clusters. It only claims whether there is a single cluster or several. Moreover unimodality testing is generally more efficient than clustering (lower complexity).

Here is presented the univariate case, but the **FTU** works also on multivariate distributions!

<div class="figure" style="width: 50%; float: left">
    <object type="image/svg+xml" data="assets/unimod.svg" style="width: 95%; float: left;">
    </object>
    <figcaption>Unimodal distribution</figcaption>
</div>

<div class="figure" style="width: 50%; float: right">
    <object type="image/svg+xml" data="assets/multimod.svg" style="width: 95%; float: right;">
    </object>
    <figcaption>Multimodal distribution</figcaption>
</div>

## Understanding the folding mechanism
<a name="mechanism"></a> 

Let us give some explanations about how the FTU works.

<div style="float: left; width: 100%; margin-bottom: 0.5em;">
    <div class="figure" style="float: left; width: 45%; margin: 0 2%;">
        <object type="image/svg+xml" data="assets/0_big_variance.svg">
        </object>
        Let us consider a bimodal distribution. Because of the two modes its variance is quite large.
    </div>
    <div class="figure" style="float: left; width: 45%; margin: 0 2%;">
        <object type="image/svg+xml" data="assets/1_pivot.svg">
        </object>
        Now, the idea is to find a "suitable" <i>pivot</i> noted s*...
    </div>
</div>

<div style="float: left; width: 100%; margin-bottom: 3em;">
    <div class="figure" style="float: left; width: 45%; margin: 0 2%;">
        <object type="image/svg+xml" data="assets/2_fold.svg"></object>
        ... so as to fold a mode (for example the left one) onto the other one.
    </div>
    <div class="figure" style="float: left; width: 45%; margin: 0 2%;">
        <object type="image/svg+xml" data="assets/3_folded_dist.svg" style="width: 100%;">
        </object>
        The resulting distribution (i.e. the sum of the right mode and the folded left mode) has then a lower variance.
    </div>
</div>

Obviously, no matter the shape of the distribution, the variance is reduced by folding. But, the reduction is far greater in the multimodal case than in the unimodal case. The FTU uses this phenomenon to discriminate unimodal distribution from multimodal ones.

## Output of the test
<a name="output"></a> 

### The folding statistics
<a name="phi"></a> 

No matter the library you use, the output results are quite the same. The main returned result is the **folding statistics** noted $\Phi$. It scores the *unimodality character* of the input data distribution. 

<div class="frame">
If $\Phi\ge1$ the distribution is rather unimodal while $\Phi< 1$ indicates it is rather multimodal.
</div>

So the final decision of the test is based on the value of $\Phi$. 

### The p-value
<a name="p-value"></a> 

Moreover, the test also outputs a **p-value** which indicates the significance of the test. The lower, the better. The folding statistics has naturally a direct impact on the p-value but the latter also depends on the size of the dataset.

Indeed, according to the amount of data you have, you have not the same knowledge about the underlying distribution, so the test has not the same relevance. Thus, the the more data you have, the more significant the test will be.

Usually, the test is considered as significant when the p-value is lower than 0.05.

### The folding pivot
<a name="pivot"></a> 

Finally, as the test needs to compute the **folding pivot** (noted $s^*$), you can retrieve this information.

## Other unimodality tests
<a name="other"></a>

Müller, D. W., & Sawitzki, G. (1991). <a href="https://www.jstor.org/stable/pdf/2290406.pdf">Excess mass estimates and tests for multimodality</a>. *Journal of the American Statistical Association*, 86(415), 738-746.

Hartigan, J. A., & Hartigan, P. M. (1985). <a href="https://projecteuclid.org/download/pdf_1/euclid.aos/1176346577">The dip test of unimodality</a>. *The annals of Statistics*, 13(1), 70-84.

Silverman, B. W. (1981). <a href="https://www.jstor.org/stable/pdf/2985156.pdf?casa_token=N0Ig7PjdpCQAAAAA:K-ekvciPzFJaTpB3P-uUUfroZtAmSnSrUdbX8TopjFWFk9Zt6-T5xcnTKJ9kNygr5hKmcJDuuGWZrBWzcRV_UCIVFR2RcGVy37bvnL6hzYDjZgE5aK0">Using kernel density estimates to investigate multimodality</a>. *Journal of the Royal Statistical Society. Series B (Methodological)*, 97-99.