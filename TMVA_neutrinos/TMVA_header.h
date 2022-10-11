#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>  // std::setprecision()

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"

#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"


		

class TMVAvar{
	TString var_type;
	int i_val;
	float  f_val;
	double d_val;
	float f4_val[4];
	//...You can add another types
	public:
		void SetType(TString type){
			var_type = type;
		}
		void SetValueAddress(TTree* tree, TString variableName){
			if (var_type == 'I')
				tree->SetBranchAddress(variableName, &i_val);
			else if (var_type == 'F')
				tree->SetBranchAddress(variableName, &f_val);
			else if (var_type == 'D')
				tree->SetBranchAddress(variableName, &d_val);
			else if (var_type == "F_arr4")
				tree->SetBranchAddress(variableName, &f4_val);
			else{
				std::cout<<"Unkown variable type!"<<std::endl;
				exit(-1);
			} 	
		}
		float GetValueAsFloat(){
			if (var_type == 'I')
				return float(i_val);
			else if (var_type == 'F')
				return f_val;
			else if (var_type == 'D')
				return float(d_val);
			else{
				std::cout<<"[ERROR] Unkown variable type!"<<std::endl;
				exit(-1);
			} 	
		}

		float GetNthValueAsFloat(unsigned int N){
			if (var_type == "F_arr4")
				return f4_val[N];
			else{
				std::cout<<"[ERROR] Wrong data type for this function!"<<std::endl;
				exit(-1);
			}
		}

};

std::vector<std::string> split(const std::string& str, char sep){
	std::istringstream ss(str);
	std::vector<std::string> v;
	std::string s;
	while(std::getline(ss, s, sep))
		v.push_back(s);
	return v;
};

std::vector<std::string> GetStartingParamsFromFile(std::string AUC_filename){


	std::vector<std::string> initial_params = {"0.0", "0.0", "0", "0.0", "0"};
	std::string s;
	std::fstream f_AUC;
	f_AUC.open(AUC_filename, std::ios::in);


	if(f_AUC.is_open()){
		std::cout<<"AUC file opened\n"<<std::endl;
		f_AUC.seekg(-2,ios_base::end); 
		bool keepLooping = true;
        while(keepLooping) {
            char ch;
            f_AUC.get(ch);                            // Get current byte's data

            if((int)f_AUC.tellg() <= 1) {             // If the data was at or before the 0th byte
                f_AUC.seekg(0);                       // The first line is the last line
                keepLooping = false;                // So stop there
            }
            else if(ch == '\n') {                   // If the data was a newline
                keepLooping = false;                // Stop at the current position.
            }
            else {                                  // If the data was neither a newline nor at the 0 byte
                f_AUC.seekg(-2,ios_base::cur);        // Move to the front of that data, then to the front of the data before it
            }
        }

		std::getline(f_AUC, s);
		std::vector<std::string> params = split(s, ' ');
		f_AUC.close();
		return params;
	}

	else{
		std::cout<<"AUC file not found\n"<<std::endl;
		f_AUC.close();
		return initial_params;
	}

	

};
