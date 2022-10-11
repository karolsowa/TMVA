#ifndef MAXAUC_H
#define MAXAUC_H
#include <cstdlib>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include "TString.h"
#include "TFile.h"
#include "TROOT.h"
#include <fstream>


class Results{
	public:
		double AUC;
		double AdaBoost;
		int MaxDepth;
		double MinNodeSize;
		int NTrees;
		Results(double a, double b, int c, double d, int e): AUC{a}, AdaBoost{b}, MaxDepth{c}, MinNodeSize{d}, NTrees{e} { }
		~Results() {}
};

bool AUC_compare(Results r1, Results r2){

	return (r1.AUC < r2.AUC);
}

#endif