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
#include <TMVA/LossFunction.h>
#include <TXMLEngine.h>
#include "TGaxis.h"
#include "TMath.h"
#include <vector>
#include <chrono>
#include <fstream>
		
Double_t BDT_classifier(double AdaBoostBeta = 0.6, int MaxDepth = 5, double MinNodeSize = 15.5, int NTrees = 2000){

// **************** PART 1 - ANALYSE ********************* //

	//Input files:
	
	TString signal_path = "/data4/muchaa/Jan2021/NTUPLE/Bd2DK0Pi_2012MC_slim.root/DecayTree";
	TString signal_path2 = "/data4/muchaa/Jan2021/NTUPLE/Bd2DK0K_2012MC_slim.root/DecayTree";
	TString background_path = "/data4/muchaa/Jan2021/B2DK0sPi_2016up_slim_B_Dsidebans.root/DecayTree";				//Remember to add all signal and bkg paths!!! 
	//TString background_path2 = "/home/student/Project/B2DK0Pi/B2D3Pi_2016MC_bkg.root/DecayTree";					//(have a look at line 129)
	TString inputTree_path = "/data4/muchaa/Jan2021/NTUPLE/B2DK0sPi_2016up_slim_v4.root/DecayTree";
	//TString inputTree_path2 = "/home/student/Project/B2DK0Pi/B2DK0Pi_2012MC_sig.root/DecayTree"; 

	//Weights directory name 
	(TMVA::gConfig().GetIONames()).fWeightFileDir = "weights_BDT";
	weights_dir = "./dataset/weights_BDT/";
	// output file with NTUPLE:
	TString outfileName = "./sample_decay_BDT.root";
	// file for post_BDT ana:
	TString postBDT_file =  "./sample_decay_postBDT.root"; 	

	std::cout<<"Macro starts"<<"\n";

	//Starting a clock to measure execution's time
	auto start = std::chrono::high_resolution_clock::now();

	std::cout << std::endl << "==> Starting TMVAClassification" << std::endl;
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
        "KS0_OWNPV_CHI2",
        "Pi_Plus_KS0_IPCHI2_OWNPV",
        "Pi_Minus_KS0_IPCHI2_OWNPV"
	};
	std::cout<<"Created a vector of variables"<<"\n";
	std::vector<TString> spectators = {};

	TFile* outputFile = TFile::Open(outfileName, "RECREATE");
	std::cout<<"Opened the output file"<<"\n";

	TMVA::Factory *factory = new TMVA::Factory("TMVAClassification", outputFile,"!V:Silent:!Color:!DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification");
	std::cout<<"Created the factory"<<"\n";
	//V- verbose flag - full information mode
	//Silent - prevents any output from TMVA after the Factory creation
	//Color - coloured screen output
	//Draw Progress Bar - display it
	//Transformations - list of transformations to test. Here: I - identity, D - decorelation D, P - PCA, G - Gaussianisation, D - decorelation
	//Analysis Type - Set  the  analysis  type  (Classification, Regression, Multiclass, Auto)
	/*used in the 'factory instantiation" these transformation are just for
	'information/visualisation/etc'. It gives you an idea at how your
	variables could be transformed to for example, make them look
	gaussian, decorrelat etc., you can plot the transformed variables and
	check them out. Sometimes, (e.g. for the projective likelihood, which
	performs best only if the variables are uncorrelated) if a
	decorrelation is possible the classifiers perform better. So it's
	something to check out. If you then decide to use any of those
	transformations in your classifier, you have to specify them in the
	booking of the classifier.*/
	TMVA::DataLoader *dataloader = new TMVA::DataLoader("dataset");
	std::cout<<"Created the dataloader"<<"\n";

	for (const TString& variable : variables) {
		dataloader->AddVariable(variable, 'F');								//Option'F' (float) is needed - ROOT have some troubles with proper reading data of type <double>
		//dataloader->AddVariable("log("+variable+")", 'F');	
	}
	for (const TString& spectator : spectators) {
		dataloader->AddSpectator(spectator, 'F');
		//dataloader->AddSpectator("log("+spectator+")", 'F');
	}

	std::cout<<"Added variables"<<"\n";

	// ---- Define signal input tree
	TChain * signal  = new TChain("signal");
	TChain * background  = new TChain("background");
	TChain * inputTree  = new TChain("inputTree");

	signal->Add(signal_path); 
	signal->Add(signal_path2);		
	background->Add(background_path);		
	inputTree->Add(inputTree_path); 

	std::cout<<"signal, bkg and input added"<<"\n";					

	Double_t signal_weight     = 1.0;
	Double_t background_weight = 1.0;

	dataloader->AddSignalTree(signal, signal_weight);
    dataloader->AddBackgroundTree(background, background_weight);

    std::cout<<"Signal and bkg tree loaded"<<"\n";

    TCut cut1 = ""; // for example: TCut mycuts = "abs(var1)<0.5 && abs(var2-0.5)<1";
	TCut cut2 = ""; // for example: TCut mycutb = "abs(var1)<0.5";

	dataloader->PrepareTrainingAndTestTree(cut1, cut2, "nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:!V");			//Czy 0 znaczą, że jest na pół?
	//SplitMode - defines how the training and test samples are selected from the source trees (Random/Alternate/Block)
	//NormMode - NumEvents: renormalisation of the training events such that the sum of event weights of the Signal and Background events, respectively
	//			 are equal to the number of events Ns,Nb requested in the call Factory::PrepareTrainingAndTestTree
	//V - verbosity
	std::cout<<"Training and test tree prepared"<<"\n";
	std::string AdaBoostBeta_str = std::to_string(AdaBoostBeta);
	std::string MaxDepth_str = std::to_string(MaxDepth);
	std::string MinNodeSize_str = std::to_string(MinNodeSize) + "%";
	std::string NTrees_str = std::to_string(NTrees);

	
	factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDTG", "!H:!V:VarTransform=N,G,G_Signal,G_Background:NTrees=" + NTrees_str + ":MinNodeSize=" + MinNodeSize_str + ":MaxDepth=" + MaxDepth_str + ":BoostType=AdaBoost:AdaBoostBeta=" + AdaBoostBeta_str + ":UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=100");
	//kBDT - unique enumerator
	//"BDTG" - user-defined name (can be different)
	//H
	//V
	//VarTransform - Gauss_Signal - Gauss_Background (transformations before testing)
	//NTrees - number of trees in the forest
	//MinNodeSize - Minimum percentage of training events required in a leaf node (default: Classification: 5%, Regression: 0.2%)
	//MaxDepth - The maximum number of terminal nodes in a tree is 2 to the power of the depth.
	//BoostType - here we use Adaptive Boost - In a classification problem, events that were misclassified 
	//			during the training of a decision tree are given a higher event weight in the training of the following tree.
	//AdaBoostBeta- the learning rate ofthe AdaBoost algorithm is controled by a parameter β giving as an exponent to the boost weight α→α^β
	//UseBaggedBoost - probably it means to use also Boosted Bagging
	//BaggedSampleFracction - Relative size of bagged event sample to original size of the data sample
	//Separation Type - Separation criterion for node splitting.  All separation criteria have a maximum where the samples are fullymixed, i.e.,
	//					at purityp= 0.5, and fall off to zero when the sample consists of one event class only.
	//					Gini Index[default], defined by p·(1−p) (p - purity)
	//nCuts - Number of grid points in variablerange used in finding optimal cut in node splitting
	std::cout<<"Method booked"<<"\n";

	factory->TrainAllMethods();
	std::cout<<"All methods trained"<<"\n";
	factory->TestAllMethods();
	std::cout<<"All methods tested"<<"\n";
	factory->EvaluateAllMethods();
	std::cout<<"All methods evaluated"<<"\n";

	//Code that allows to obtain BDT reference cut value and AUC value, which is returned at the end of the programme execution-------------

	auto imethod = factory->GetMethod("dataset", "BDTG");
	auto method = dynamic_cast<TMVA::MethodBase *>(imethod);

	Double_t cut = method->GetSignalReferenceCut();
	Double_t AUC = factory->GetROCIntegral(dataloader, "BDTG");

	outputFile->Close();
	std::cout<<"File closed"<<"\n";
	delete factory;

	//--------------------------------------------------------------------------------------------------------------------------------------

	std::cout << "==> Starting TMVAClassificationApplication" << std::endl;
	//Creating reader based on weight file 
	TMVA::Reader *reader = new TMVA::Reader("!Color:!Silent");
	//The only options are the booleans:Vfor verbose,Color for coloured output, and Silent to suppress all output
	TObjArray *branches= inputTree->GetListOfBranches();
	unsigned long size = (unsigned long) branches->GetEntries();

	std::vector<Double_t> values_variables(variables.size());
	std::vector<Double_t> values_spectators(spectators.size());


	for (unsigned i= 0; i < variables.size(); ++i) {
		TString variableName = variables[i];
      	inputTree->SetBranchAddress(variableName, &values_variables[i]);		//Errors may appear, but we can do nothing about it
	}
	for (unsigned i= 0; i < spectators.size(); ++i) {
		TString variableName = spectators[i];
		inputTree->SetBranchAddress(variableName, &values_spectators[i]);
	}

	std::vector<Float_t> values_variables_reader(variables.size());
	std::vector<Float_t> values_spectators_reader(spectators.size());

	for (unsigned i = 0; i < variables.size(); ++i) {
		reader->AddVariable(variables[i], &values_variables_reader[i]);
		//reader->AddVariable("log("+variables[i]+")", &values_variables_reader[i]);
	}
	for (unsigned i = 0; i < spectators.size(); ++i) {
		reader->AddSpectator(spectators[i], &values_spectators_reader[i]);
		//reader->AddSpectator("log("+variables[i]+")", &values_spectators_reader[i]);
	}

	Double_t BDT_response; 
	TString dir2    = weights_dir;
	TString prefix2 = "TMVAClassification";
	TString methodName2 = "BDTG method";
	TString weightfile2 = dir2 + prefix2 + "_BDTG.weights.xml";


	reader->BookMVA(methodName2, weightfile2);
	Int_t nEvent2 = (Int_t) inputTree->GetEntries();				//Here, you can decide if you want to run your programme on a full sample or just test it on 1000 events
	//Int_t nEvent2 = 1000;

	TFile *newfile = new TFile(postBDT_file,"recreate");					//post BDT analysis file creation
	TTree *newtree = inputTree->CloneTree(0);						
	TBranch *myBDTr = newtree->Branch("BDT_response", &BDT_response);

	for (Long64_t ievt2=0; ievt2<nEvent2; ievt2++) {
		inputTree->GetEntry(ievt2);
		if(ievt2 % 10000 == 0) cout << " Processed: " << ievt2 << " entries " <<endl;			
		for (unsigned j=0; j<variables.size(); j++) 
				values_variables_reader[j]=(float)values_variables[j];     

		for (unsigned j=0; j<spectators.size(); j++)
				values_spectators_reader[j]=(float)values_spectators[j];
		BDT_response = reader->EvaluateMVA("BDTG method");
		newtree->Fill();

	}	
	newfile->Write();
	delete newfile;
	delete reader;
	//Stops the clock's countdown
	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;

	//Final results 
	std::cout<<"BDT  cut: "<<cut<<"\n";
	std::cout << "Elapsed time: " << floor(elapsed.count()/60) << " min "<< fmod(elapsed.count(), 60) << " s\n";
	std::cout<<"AUC / ROC integral: "<<AUC<<"\n";
	return AUC;
	
}
