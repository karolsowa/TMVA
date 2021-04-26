#include "GridSearch.h"
#include "Cascade1.C"
void GridSearch(Double_t AdaBoostBetaValue = 0.01, Int_t MaxDepthStep = 1, Double_t MinNodeSizeStep = 0.02, Int_t NTreesStep = 50){

	// for(int i = 1; AdaBoostBeta.at(i-1) <= 0.6 - AdaBoostBetaStep; i++){			// Odkomentować przy zwykłym uruchamianiu
	// 	Double_t s = AdaBoostBeta.at(0) + i*AdaBoostBetaStep;
	// 	AdaBoostBeta.push_back(s);
	// }

	AdaBoostBeta.at(0) = AdaBoostBetaValue;																			// Odkomentować przy zrównoleglaniu
	//srand(time(NULL));

	for(int i = 1; MaxDepth.at(i-1) <= 5 - MaxDepthStep; i++){
		Int_t s = MaxDepth.at(0) + i*MaxDepthStep;
		MaxDepth.push_back(s);
	}


	for(int i = 1; MinNodeSize.at(i-1) <= 0.2 - MinNodeSizeStep; i++){
		Double_t s = MinNodeSize.at(0) + i*MinNodeSizeStep;
		MinNodeSize.push_back(s);
	}


	for(int i = 1; NTrees.at(i-1) <= 650 - NTreesStep; i++){
		Int_t s = NTrees.at(0) + i*NTreesStep;
		NTrees.push_back(s);
	}


	for(unsigned int i = 0; i < AdaBoostBeta.size(); i++)
		std::cout<<AdaBoostBeta.at(i)<<' ';
	std::cout<<'\n';
	for(unsigned int i = 0; i < MaxDepth.size(); i++)
		std::cout<<MaxDepth.at(i)<<' ';
	std::cout<<'\n';
	for(unsigned int i = 0; i < MinNodeSize.size(); i++)
		std::cout<<MinNodeSize.at(i)<<' ';
	std::cout<<'\n';
	for(unsigned int i = 0; i < NTrees.size(); i++)
		std::cout<<NTrees.at(i)<<' ';
	std::cout<<'\n';

	std::cout<<"AdaBoostBeta: "<<AdaBoostBeta.size()<<'\n';
	std::cout<<"MaxDepth: "<<MaxDepth.size()<<'\n';
	std::cout<<"MinNodeSize: "<<MinNodeSize.size()<<'\n';
	std::cout<<"NTrees: "<<NTrees.size()<<'\n';

	std::cout<<"Liczba kombinacji: "<<AdaBoostBeta.size()*MaxDepth.size()*MinNodeSize.size()*NTrees.size()<<'\n';
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));

	Double_t name_file_d = round(AdaBoostBetaValue*100);
	Int_t name_file = int(name_file_d);
	std::string name_str = std::to_string(name_file);
	std::string file_str = "/home/sowrol/repo/outfiles/outfile_" + name_str + ".txt";
	//std::string error_str = "/home/sowrol/repo/outfiles/outfile_" + name_str + "_error.txt";
	//std::ofstream errorfile(error_str.c_str(), ios_base::app);
	std::ofstream outfile;

	for(int i = 0; i < AdaBoostBeta.size(); ++i){
		for(int j = 3; j < MaxDepth.size(); ++j){
			for(int k = 7; k < MinNodeSize.size(); ++k){
				for(int l = 0; l < NTrees.size(); ++l){
	// int i = 0;

					outfile.open(file_str.c_str(), ios_base::app);
					if (!outfile.is_open()) std::cout<<"Błąd otwarcia pliku do zapisu";	//errorfile<<
	// int j = 0;
	// int k = 0;
	// int l = 0;
					Double_t AUC = Cascade1(AdaBoostBeta.at(i), MaxDepth.at(j), MinNodeSize.at(k), NTrees.at(l)); //rand()/double(RAND_MAX)
					Result R(AUC, AdaBoostBeta.at(i), MaxDepth.at(j), MinNodeSize.at(k), NTrees.at(l));
					Results.push_back(R);
					outfile<<R.AUC<<' '<<R.AdaBB<<' '<<R.MaxD<<' '<<R.MinNS<<' '<<R.NT<<"\n";
					outfile.close();
	//Double_t AUC = 0.1;
	//Result R(AUC, AdaBoostBeta.at(0), MaxDepth.at(0), MinNodeSize.at(0), NTrees.at(0));
				}
			}
		}
	}

	
	outfile.open(file_str.c_str(), ios_base::app);
	std::cout<<"[INFO]:: End of writing to file: outfile_"<<name_str<<".txt"<<"\n";
	outfile<<"[INFO]:: End of writing to file: outfile_"<<name_str<<".txt"<<"\n";
	outfile.close();
	//errorfile.close();
	
	//auto it = max_element(Results.begin(), Results.end());
	//std::cout<<"Best AUC value: "<<*it<<"\n";
	//std::cout<<"For parameters: "<<"\n";

	//R.Print();

	// double a = 1;
	// double b = 7;
	// std::string a_str = std::to_string(a);
	// std::string b_str = std::to_string(b);
	// std::string line = ".x test.C(" + a_str + "," + b_str + ")";
	// gROOT->ProcessLine(line.c_str());
	// Double_t AUC = Cascade1();
	// std::cout<<"AUC = "<<AUC;


	
	//AdaBoostBeta.size();
	// for(int i=0; i < AdaBoostBeta.size(); i++){
	// 	std::cout<<AdaBoostBeta.at(i)<< ' ';
	// }
}

