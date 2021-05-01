#ifndef GRIDSEARCH_H
#define GRIDSEARCH_H
#include <cstdlib>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include "TString.h"
#include "TFile.h"
#include "TROOT.h"
#include <fstream>

class Result{
	public:
		Double_t AUC;
		Double_t AdaBB;
		Int_t MaxD;
		Double_t MinNS;
		Int_t NT;
		Result(Double_t a, Double_t b, Int_t c, Double_t d, Int_t e): AUC{a}, AdaBB{b}, MaxD{c}, MinNS{d}, NT{e} {};
		~Result() {};
		void Print() const; 
};

std::vector<Double_t> AdaBoostBeta = {0.01};
std::vector<Int_t> MaxDepth = {2};
std::vector<Double_t> MinNodeSize = {5.0};
std::vector<Int_t> NTrees = {450};
std::vector<Result> Results;

void Result::Print() const{
	std::cout<<"AUC  = "<<AUC<<"\n";
	std::cout<<"AdaBoostBeta  = "<<AdaBB<<"\n";
	std::cout<<"MaxDepth  = "<<MaxD<<"\n";
	std::cout<<"MinNodeSize  = "<<MinNS<<"\n";
	std::cout<<"NTrees  = "<<NT<<"\n";
}

#endif
