#include "MaxAUC_neutrinos.h"
#include "TMVA_neutrinos.C"

const Results MaxAUC(std::string filename){

	std::ifstream inputfile(filename);
	if (!inputfile.is_open()) std::cout<<"Error opening input file!\n";
	std::string s;
	std::vector<std::string> elements;
	std::vector<double> AUC;
	std::vector<double> AdaBoost;
	std::vector<int> MaxDepth;
	std::vector<double> MinNodeSize;
	std::vector<int> NTrees;
	while (std::getline(inputfile, s)){
		std::vector<std::string> tmp = split(s, ' ');
		AUC.push_back(std::stod(tmp.at(0)));
		AdaBoost.push_back(std::stod(tmp.at(1)));
		MaxDepth.push_back(std::stoi(tmp.at(2)));
		MinNodeSize.push_back(std::stod(tmp.at(3)));
		NTrees.push_back(std::stoi(tmp.at(4)));
	}


	auto result = std::max_element(AUC.begin(), AUC.end());
	long index = result - AUC.begin();
	std::cout<<"Max AUC in file: "<<*result<<'\n';

	Results R(AUC.at(index), AdaBoost.at(index), MaxDepth.at(index), MinNodeSize.at(index), NTrees.at(index));
	return R;

}

