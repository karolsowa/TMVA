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

    //You should only change the following lines: (and line 121)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- 

// ---- Add input files
    TChain * signal  = new TChain("signal");
    TChain * background  = new TChain("background");
    TChain * inputTree  = new TChain("inputTree");

    signal->Add("/data4/muchaa/Jan2021/NTUPLE/Bd2DK0Pi_2012MC_slim.root/DecayTree"); 
    signal->Add("/data4/muchaa/Jan2021/NTUPLE/Bd2DK0K_2012MC_slim.root/DecayTree");
    //background->Add("/home/student/Project/B2DK0Pi/B2D3Pi_2016MC_bkg.root/DecayTree");            
    background->Add("/data4/muchaa/Jan2021/B2DK0sPi_2016up_slim_B_Dsidebans.root/DecayTree");       
    inputTree->Add("/data4/muchaa/Jan2021/NTUPLE/B2DK0sPi_2016up_slim_v4.root/DecayTree"); 

    // output file with classifier performance results:
    TString outfileName = "/home/sowrol/repo/TMVA_cascade1_HPO.root";

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

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
        "D0_FD_OWNPV",
        "D0_FD_ORIVX",
        "D0_FDCHI2_ORIVX",
        "D0_OWNPV_CHI2",
        "maxD_GhostProb",
        "minIPchi2_D_childs",
        "D0_Dist_z",
        "maxK0_GhostProb",
        "minIPchi2_K0_childs",
        "deltaR_Ks0Pi",                 
        "KS0_ENDVERTEX_CHI2",
        "KS0_PT",
        "KS0_IP_OWNPV",
        "KS0_IPCHI2_OWNPV",
        "KS0_FD_OWNPV",
        "KS0_FDCHI2_OWNPV",
        "KS0_FD_ORIVX",
        "KS0_FDCHI2_ORIVX",
        "KS0_ORIVX_CHI2",
//        "KS0_DIRA_ORIVX",
//        "KS0_FDCHI2_OWNPV",
        "KS0_OWNPV_CHI2",
        "Pi_Plus_KS0_IPCHI2_OWNPV",
        "Pi_Minus_KS0_IPCHI2_OWNPV"
    //    "KS0_CosTheta"
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
	TFile* outputFile = TFile::Open(outfileName, "RECREATE");

	TMVA::Factory *factory = new TMVA::Factory("TMVAClassification", outputFile,"!V:!Silent:!Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification");
	TMVA::DataLoader *dataloader = new TMVA::DataLoader("dataset");

	for (const TString& variable : variables) {
		dataloader->AddVariable(variable, 'F');								
		//dataloader->AddVariable("log("+variable+")", 'F');	
	}
	for (const TString& spectator : spectators) {
		dataloader->AddSpectator(spectator, 'F');
		//dataloader->AddSpectator("log("+spectator+")", 'F');
	}
							

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
    std::cout<<"All done"<<"\n";

}