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
		
void Optimize(){

// **************** PART 1 - ANALYSE ********************* //

	std::cout << std::endl << "==>TMVAClassification: FIRST CLASSIFIER" << std::endl;
	std::vector<TString> variables = {
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
//		"deltaR_Ks0Pi", 				//później
//		"KS0_CosTheta",
//		"K_Plus_D_PIDK",
//		"K_Minus_D_PIDK",
//		"Pi_Minus_D_PIDK",
//		"Pi_Plus_KS0_PIDK",
//		"Pi_Minus_KS0_PIDK",
//		"Pi_Plus_PIDK" 

	};
	std::vector<TString> spectators = {};
	TString outfileName = "/home/student/Project/B2DK0Pi/ANALIZA/TMVA_cascade1.root";
	TFile* outputFile = TFile::Open(outfileName, "RECREATE");

	TMVA::Factory *factory = new TMVA::Factory("TMVAClassification", outputFile,"!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification");
	TMVA::DataLoader *dataloader = new TMVA::DataLoader("dataset");

	for (const TString& variable : variables) {
		dataloader->AddVariable(variable, 'F');								
		//dataloader->AddVariable("log("+variable+")", 'F');	
	}
	for (const TString& spectator : spectators) {
		dataloader->AddSpectator(spectator, 'F');
		//dataloader->AddSpectator("log("+spectator+")", 'F');
	}

	// ---- Define signal input tree
	TChain * signal  = new TChain("signal");
	TChain * background  = new TChain("background");
	TChain * inputTree  = new TChain("inputTree");

	signal->Add("/home/student/Project/B2DK0Pi/B2DK0Pi_2012MC_sig.root/DecayTree"); 
	background->Add("/home/student/Project/B2DK0Pi/B2DX_2016_job_2_sidebands.root/DecayTree");		
	inputTree->Add("/home/student/Project/B2DK0Pi/B2DX_2016_job_2.root/Bd2DK0PiTree/DecayTree"); 


	// file for post_BDT ana:
	TString myfile =  "/home/student/Project/B2DK0Pi/ANALIZA/MyBDT_cascade1.root"; 							

	Double_t signal_weight     = 1.0;
	Double_t background_weight = 1.0;

	dataloader->AddSignalTree(signal, signal_weight);
    dataloader->AddBackgroundTree(background, background_weight);

    TCut cut1 = ""; // for example: TCut mycuts = "abs(var1)<0.5 && abs(var2-0.5)<1";
	TCut cut2 = ""; // for example: TCut mycutb = "abs(var1)<0.5";

	TMVA::HyperParameterOptimisation * HPO = new TMVA::HyperParameterOptimisation(dataloader);

	HPO->BookMethod(TMVA::Types::kBDT, "BDTG", "");
	HPO->SetNumFolds(4);

	std::cout << "Info: calling TMVA::HyperParameterOptimisation::Evaluate" << std::endl;
     HPO->Evaluate();
     TMVA::HyperParameterOptimisationResult HPOResult = HPO->GetResults();
     HPOResult.Print();

	//dataloader->PrepareTrainingAndTestTree(cut1, cut2, "nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:!V");			
//   TString methodOptions = "!H:!V:VarTransform=N,G,G_Signal,G_Background:NTrees=2000:MinNodeSize=2.5%:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=100";
//   auto method = factory->BookMethod(dataloader, TMVA::Types::kBDT,"BDTG", methodOptions);
//	 method->OptimizeTuningParameters();
	//factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDTG", "!H:!V:VarTransform=N,G,G_Signal,G_Background:NTrees=2000:MinNodeSize=2.5%:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=100");



}