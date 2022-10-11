#include "MaxAUC_neutrinos.C"


void RunMaxAUC_neutrinos(bool mode){
	
    std::string filename = "AUC.txt";
    auto maxAUC = MaxAUC(filename);
    std::cout<<"\nMax AUC in all files: "<<(maxAUC).AUC<<"\n";

    if (mode){
    	std::cout<<"\nRunning the classifier for optimal parameters...\n";
    // 	Cascade1((*maxAUC).AdaBoost,(*maxAUC).MaxDepth, (*maxAUC).MinNodeSize, (*maxAUC).NTrees);
    }
    else
    	std::cout<<(maxAUC).AdaBoost<<"  "<<(maxAUC).MaxDepth<<"  "<<(maxAUC).MinNodeSize<<"  "<<(maxAUC).NTrees<<"\n";
}
