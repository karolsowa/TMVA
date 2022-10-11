#include "TMVA_neutrinos.C"

int RunTMVA(){

	int status = 0;
	while (status == 0){
		status = TMVA_neutrinos();
	}

	return 0;
}