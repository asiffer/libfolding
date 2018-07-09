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
	
	double sf_folding_test(StreamFolding * sf, bool *unimodal, double *p_value) {
		return sf->folding_test(unimodal, p_value);
	}
	
	/*
	double * sf_mean(StreamFolding * sf) {
		return sf->mean().memptr();
	}
	
	double * sf_cov(StreamFolding * sf) {
		return sf->cov().memptr();
	}
	
	double * sf_s2star(StreamFolding * sf) {
		return sf->s2star().memptr();
	}
	
	double * sf_dump(StreamFolding * sf) {
		return sf->dump().memptr();
	}
	*/
	void sf_mean(StreamFolding * sf, double * output) {
		output = sf->mean().memptr();
	}
	
	void sf_cov(StreamFolding * sf, double * output) {
		output = sf->cov().memptr();
	}
	
	void sf_s2star(StreamFolding * sf, double * output) {
		output = sf->s2star().memptr();
	}
	
	void sf_dump(StreamFolding * sf, double * output) {
		output = sf->dump().memptr();
	}
}

