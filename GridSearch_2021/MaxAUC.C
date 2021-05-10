#include "MaxAUC.h"





const Results MaxAUC(int value){

	std::string str_value = std::to_string(value);
	std::ifstream inputfile("/home/sowrol/repo/outfiles/outfile_" + str_value + ".txt");
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
		//std::cout<<tmp.at(0)<<"\n";
	}


	auto result = std::max_element(AUC.begin(), AUC.end());
	long index = result - AUC.begin();
	std::cout<<"Max AUC in outfile_"<<value<<": "<<*result<<'\n';

	Results R(AUC.at(index), AdaBoost.at(index), MaxDepth.at(index), MinNodeSize.at(index), NTrees.at(index));
	return R;

}

