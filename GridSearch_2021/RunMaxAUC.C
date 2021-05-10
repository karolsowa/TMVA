#include "MaxAUC.C"
#include "/home/student/Project/B2DK0Pi/repo/GridSearch_2021/Cascade1.C"

void RunMaxAUC(int mode = 0){
	
	int AdaBoostStep = 5;
    std::vector<int> AdaBoost = {1};
    std::vector<Results> VR;
    for(int AdaBoost = 1; AdaBoost < 60;  AdaBoost += AdaBoostStep){
    	VR.push_back(MaxAUC(AdaBoost));
    }

    auto maxAUC = std::max_element(VR.begin(), VR.end(), AUC_compare);
    std::cout<<"\nMax AUC in all files: "<<(*maxAUC).AUC<<"\n";

    if (mode){
    	std::cout<<"\nRunning the classifier for optimal parameters...\n";
    	Cascade1((*maxAUC).AdaBoost,(*maxAUC).MaxDepth, (*maxAUC).MinNodeSize, (*maxAUC).NTrees);
    }
    else
    	std::cout<<(*maxAUC).AdaBoost<<"  "<<(*maxAUC).MaxDepth<<"  "<<(*maxAUC).MinNodeSize<<"  "<<(*maxAUC).NTrees<<"\n";
}