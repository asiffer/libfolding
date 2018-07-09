/**
	@file interface.cpp
	@brief C interface (for python notably)
	@author Alban Siffer
	@date 05 July 2018
*/

#include "streamfolding.h"

using namespace std;

extern "C" {
	StreamFolding * sf_new(size_t depth) { 
		return new StreamFolding(depth);
	}
	
	void sf_update(StreamFolding * sf, double * array_ptr, int n_elem, bool copy, bool strict) {
		sf->update(arma::vec(array_ptr, n_elem, copy, strict));
	}
	
	bool sf_is_initialized(StreamFolding * sf) {
		return sf->is_initialized();
	}
	
	void sf_folding_test(StreamFolding * sf, bool *unimodal, double *p_value, double * Phi) {
		*Phi = sf->folding_test(unimodal, p_value);
	}
	

	void sf_mean(StreamFolding * sf, double * output) {
		arma::vec m = sf->mean();
		for(unsigned int i = 0; i<m.size(); i++) {
			output[i] = m[i];	
		}
	}
	
	
	void sf_cov(StreamFolding * sf, double * output) {
		arma::mat c = sf->cov();
		int d = c.n_rows;
		int k = 0;
		for(int i = 0; i<d; i++) {
			for(int j = 0; j<d; j++) {
				output[k] = c(i,j);
				k++;
			}
		}
	}
	
	
	void sf_s2star(StreamFolding * sf, double * output) {
		arma::vec s = sf->s2star();
		for(unsigned int i = 0; i<s.size(); i++) {
			output[i] = s[i];	
		}
	}
	
	
	void sf_dump(StreamFolding * sf, double * output) {
		arma::mat X = sf->dump();
		int n = X.n_rows;
		int p = X.n_cols;
		int k = 0;
		for(int i = 0; i<n; i++) {
			for(int j = 0; j<p; j++) {
				output[k] = X(i,j);
				k++;
			}
		}
	}
}

