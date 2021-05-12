#include <iostream>
#include <map>
#include <string>
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TMVA/MethodCategory.h"
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"
#include "TMVA/Reader.h"
#include "TGaxis.h"
#include "TMath.h"
#include <vector>
		
void Optimize2(){
	std::cout << std::endl << "==>TMVAClassification: SECOND CLASSIFIER" << std::endl;

	std::vector<TString> variables2 = {
		"B_ENDVERTEX_CHI2",
        "B_OWNPV_CHI2",
        "B_IPCHI2_OWNPV",
        "B_DIRA_OWNPV",
        "B_PT",
        "B_FDCHI2_OWNPV",
        "D0_IPCHI2_OWNPV",
        "D0_IP_OWNPV",
        "D0_DIRA_ORIVX",
        "D0_ENDVERTEX_CHI2",
        "D0_FDCHI2_OWNPV",
        "D0_DIRA_OWNPV",
        "D0_FD_OWNPV",
        "D0_FD_ORIVX",
        "D0_FDCHI2_ORIVX",
        "D0_OWNPV_CHI2",
        "maxD_GhostProb",
        "minIPchi2_D_childs",
        "D0_Dist_z",
//		"deltaR_Ks0Pi", 				//później
//		"KS0_CosTheta",
//		"K_Plus_D_PIDK",
//		"K_Minus_D_PIDK",
//		"Pi_Minus_D_PIDK",
//		"Pi_Plus_KS0_PIDK",
//		"Pi_Minus_KS0_PIDK",
//		"Pi_Plus_PIDK" 

	};
	std::vector<TString> spectators2 = {};
	TString outfileName2 = "/home/sowrol/repo/TMVA_cascade1_OTP.root";
	TFile* outputFile2 = TFile::Open(outfileName2, "RECREATE");

	TMVA::Factory* factory2 = new TMVA::Factory("TMVAClassification", outputFile2,"!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification");
	TMVA::DataLoader* dataloader2 = new TMVA::DataLoader("dataset");


	for (const TString& variable2 : variables2) {
	 	dataloader2->AddVariable(variable2, 'F');							
	 	//dataloader2->AddVariable("log("+variable2+")", 'F');	
	}
	for (const TString& spectator2 : spectators2) {
		dataloader2->AddSpectator(spectator2, 'F');
	 	//dataloader2->AddSpectator("log("+spectator2+")", 'F');
	}

	TChain* signal2  = new TChain("signal2");
	TChain* background2  = new TChain("background2");
	TChain* inputTree2  = new TChain("inputTree2");

	signal2->Add("/data4/muchaa/Jan2021/NTUPLE/Bd2DK0Pi_2012MC_slim.root/DecayTree"); 
	signal2->Add("/data4/muchaa/Jan2021/NTUPLE/Bd2DK0K_2012MC_slim.root/DecayTree");
	//background->Add("/home/student/Project/B2DK0Pi/B2D3Pi_2016MC_bkg.root/DecayTree");			//Jako input dać MC signal 2012
	background2->Add("/data4/muchaa/Jan2021/B2DK0sPi_2016up_slim_B_Dsidebans.root/DecayTree");		//Przetestować dla obydwu
	inputTree2->Add("/data4/muchaa/Jan2021/NTUPLE/B2DK0sPi_2016up_slim_v4.root/DecayTree"); 

	TString myfile2 =  "/home/student/Project/B2DK0Pi/ANALIZA/MyBDT_cascade1_and_2.root";

	Double_t signal_weight2     = 1.0;
	Double_t background_weight2 = 1.0;

	dataloader2->AddSignalTree(signal2, signal_weight2);
    dataloader2->AddBackgroundTree(background2, background_weight2);

    TCut cut3 = ""; // for example: TCut mycuts = "abs(var1)<0.5 && abs(var2-0.5)<1";
	TCut cut4 = ""; // for example: TCut mycutb = "abs(var1)<0.5";

	// TMVA::HyperParameterOptimisation * HPO2 = new TMVA::HyperParameterOptimisation(dataloader2);

	// HPO2->BookMethod(TMVA::Types::kBDT, "BDTG", "");
	// HPO2->SetNumFolds(2);

	// std::cout << "Info: calling TMVA::HyperParameterOptimisation::Evaluate" << std::endl;
 //    HPO2->Evaluate();
 //    TMVA::HyperParameterOptimisationResult HPO2Result = HPO2->GetResults();
 //    HPO2Result.Print();

	TString methodOptions2 = "!H:!V:VarTransform=N,G,G_Signal,G_Background:NTrees=2000:MinNodeSize=2.5%:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=100";
    auto method2 = factory2->BookMethod(dataloader2, TMVA::Types::kBDT,"BDTG", methodOptions2);
	method2->OptimizeTuningParameters();
    
    //dataloader2->PrepareTrainingAndTestTree(cut3, cut4, "nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:!V");
    //factory2->BookMethod(dataloader2, TMVA::Types::kBDT, "BDTG", "!H:!V:VarTransform=N,G,G_Signal,G_Background:NTrees=2000:MinNodeSize=2.5%:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=100");
}