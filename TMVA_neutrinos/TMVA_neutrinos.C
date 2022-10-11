#include "TMVA_header.h"

int TMVA_neutrinos()
{
// **************** PART 1 - ANALYSE ********************* //


   std::cout<<"Macro starts"<<"\n";


   // You should only change the following lines:

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- 

   //Input files:
   
   TString signal_path = "/home/student/Summer_School_IFJ/Analysis  T2K/Signal.root/default";
   TString background_path = "/home/student/Summer_School_IFJ/Analysis  T2K/Background.root/default";          //Remember to add all signal and bkg paths!!!   
   TString inputTree_path = "/home/student/Summer_School_IFJ/Analysis  T2K/Prod6BNeut2w.root/default";

   // ---- Add trees to respective chains:
   TChain * signal  = new TChain("signal");
   TChain * background  = new TChain("background");
   TChain * inputTree  = new TChain("inputTree");

   signal->Add(signal_path);    
   background->Add(background_path);      
   inputTree->Add(inputTree_path); 

   std::cout<<"signal, bkg and input added"<<"\n"; 

   // output file with classifier performance results:
   TString outfileName = "./classifier_performance.root";
   // output file with NTUPLE for post_BDT ana:
   TString postBDT_file =  "./postTMVA.root";   

   std::string AUC_filename =  "./AUC.txt"; 
   std::vector<std::string> params = GetStartingParamsFromFile(AUC_filename);

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

   //Starting a clock to measure execution's time
   auto start = std::chrono::high_resolution_clock::now();

   std::cout << std::endl << "==> Starting TMVAClassification" << std::endl;
   std::vector<TString> variables = {
      "selmu_likemu",
      "selmu_pos",
      "nTPCMIPs",
      "nP0DMichel",
      "nP0DContainedMIPs",
      "nP0DShowers",
      "nP0DTPC1",
      "nproton"
      //"nmeson",
      //"selmu_likemip",
      //"selmu_ecal_det",
      //"selmu_momt" ,
      //"np0d"
   };

   //nmeson
   //selmu_likemip
   //selmu_ecal_det
   //selmu_momt ??
   //nP0Dstd::vector<std::string> params = GetStartingParamsFromFile(AUC_filename);


   int n_vars = 0;

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

   char var_type;

   for (const TString& variable : variables) {
      var_type = inputTree->GetLeaf(variable)->GetTypeName()[0];
      if(variable == "selmu_pos"){
         for(int i = 0; i < 3; i++){
            TString index = std::to_string(i);
            TString var_str = variable + "[" + index + "]";
            dataloader->AddVariable(var_str, var_type);
            n_vars += 1;
         }
      }
      else{
         dataloader->AddVariable(variable, var_type);                      //Option'F' (float) is needed - ROOT have some troubles with proper reading data of type <double>
         n_vars += 1;
      }
      
      //dataloader->AddVariable("log("+variable+")", 'F');  
   }
   for (const TString& spectator : spectators) {
      var_type = inputTree->GetLeaf(spectator)->GetTypeName()[0];
      dataloader->AddSpectator(spectator, var_type);
      //dataloader->AddSpectator("log("+spectator+")", 'F');
   }

   std::cout<<"Added variables"<<"\n";          

   Double_t signal_weight     = 1.0;
   Double_t background_weight = 1.0;

   dataloader->AddSignalTree(signal, signal_weight);
   dataloader->AddBackgroundTree(background, background_weight);

   std::cout<<"Signal and bkg tree loaded"<<"\n";

   TCut cut1 = ""; // for example: TCut mycuts = "abs(var1)<0.5 && abs(var2-0.5)<1";
   TCut cut2 = ""; // for example: TCut mycutb = "abs(var1)<0.5";

   dataloader->PrepareTrainingAndTestTree(cut1, cut2, "nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:!V");         //Czy 0 znaczą, że jest na pół?
   //SplitMode - defines how the training and test samples are selected from the source trees (Random/Alternate/Block)
   //NormMode - NumEvents: renormalisation of the training events such that the sum of event weights of the Signal and Background events, respectively
   //        are equal to the number of events Ns,Nb requested in the call Factory::PrepareTrainingAndTestTree
   //V - verbosity
   std::cout<<"Training and test tree prepared"<<"\n";

   //Set 4 important hyperparameters:

   // double AdaBoostBeta = 0.6;
   // int MaxDepth = 5;
   // double MinNodeSize = 15.5;
   // int NTrees = 2000;

   std::vector<std::string> initial_params = {"0.0", "0.0", "0", "0.0", "0"};

   std::string AdaBoostBeta_str = "0.01";
   std::string MaxDepth_str = "2";
   std::string MinNodeSize_str = "5.0%";
   std::string NTrees_str = "450";

   double MinNodeSize, AdaBoostBeta;
   
   if (initial_params != params){

      AdaBoostBeta_str = params.at(1);
      MaxDepth_str = params.at(2);
      MinNodeSize_str = params.at(3);
      NTrees_str = std::to_string(std::stoi(params.at(4)) + 50);
      if (std::stoi(params.at(4)) == 1000){
         NTrees_str = "450";
         std::ostringstream stream_MNSize;
         stream_MNSize << std::fixed;
         stream_MNSize << std::setprecision(1);
         MinNodeSize = std::stod(params.at(3)) + 2.0;
         stream_MNSize << MinNodeSize;
         MinNodeSize_str = stream_MNSize.str() + "%";
         if (std::stod(params.at(3)) > 18.0){
            MinNodeSize_str = "5.0%";
            MaxDepth_str = std::to_string(std::stoi(params.at(2)) + 1);
            if (std::stoi(params.at(2)) == 5){
               MaxDepth_str = "2";
               std::ostringstream stream_Ada;
               stream_Ada << std::fixed;
               stream_Ada << std::setprecision(2);
               AdaBoostBeta = std::stod(params.at(1)) + 0.05;
               stream_Ada << AdaBoostBeta;
               AdaBoostBeta_str = stream_Ada.str();
               if (std::stod(params.at(1)) > 0.7){
                  std::cout<<"Gridsearch ended"<<std::endl;
                  return 1;
               }
            }
         }
      }      
   }

   

   // std::cout<<AdaBoostBeta_str<<std::endl;
   // std::cout<<MaxDepth_str<<std::endl;
   // std::cout<<MinNodeSize_str<<std::endl;
   // std::cout<<NTrees_str<<std::endl;
   

   factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDTG", "!H:!V:VarTransform=N,G,G_Signal,G_Background:NTrees=" + NTrees_str + ":MinNodeSize=" + MinNodeSize_str + ":MaxDepth=" + MaxDepth_str + ":BoostType=AdaBoost:AdaBoostBeta=" + AdaBoostBeta_str + ":UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=100");
   //factory->BookMethod(dataloader, TMVA::Types::kPDERS, "PDERS","!H:V:NormTree=T:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600");
   //factory->BookMethod(dataloader, TMVA::Types::kPDEFoam, "PDEFoam","H:V:SigBgSeparate=F:TailCut=0.001:VolFrac=0.3:nActiveCells=500:nSampl=5000:nBin=5:Nmin=40:Kernel=None:Compress=T");
   //kBDT - unique enumerator
   //"BDTG" - user-defined name (can be different)
   //H
   //V
   //VarTransform - Gauss_Signal - Gauss_Background (transformations before testing)
   //NTrees - number of trees in the forest
   //MinNodeSize - Minimum percentage of training events required in a leaf node (default: Classification: 5%, Regression: 0.2%)
   //MaxDepth - The maximum number of terminal nodes in a tree is 2 to the power of the depth.
   //BoostType - here we use Adaptive Boost - In a classification problem, events that were misclassified 
   //       during the training of a decision tree are given a higher event weight in the training of the following tree.
   //AdaBoostBeta- the learning rate ofthe AdaBoost algorithm is controled by a parameter β giving as an exponent to the boost weight α→α^β
   //UseBaggedBoost - probably it means to use also Boosted Bagging
   //BaggedSampleFracction - Relative size of bagged event sample to original size of the data sample
   //Separation Type - Separation criterion for node splitting.  All separation criteria have a maximum where the samples are fullymixed, i.e.,
   //             at purityp= 0.5, and fall off to zero when the sample consists of one event class only.
   //             Gini Index[default], defined by p·(1−p) (p - purity)
   //nCuts - Number of grid points in variablerange used in finding optimal cut in node splitting
   std::cout<<"All methods booked"<<"\n";

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
   std::string AUC_str = std::to_string(AUC);
   outputFile->Close();
   std::cout<<"File closed"<<"\n";

   std::fstream f_AUC;
   f_AUC.open(AUC_filename, std::ios::app);
   f_AUC<<AUC_str<<' '<<AdaBoostBeta_str<<' '<<MaxDepth_str<<' '<<MinNodeSize_str<<' '<<NTrees_str<<"\n";
   f_AUC.close();


   delete factory;

   // //--------------------------------------------------------------------------------------------------------------------------------------

   // std::cout << "==> Starting TMVAClassificationApplication" << std::endl;
   // //Creating reader based on weight file 
   // TMVA::Reader *reader = new TMVA::Reader("!Color:!Silent");
   // //The only options are the booleans:Vfor verbose,Color for coloured output, and Silent to suppress all output
   // TObjArray *branches= inputTree->GetListOfBranches();
   // unsigned long size = (unsigned long) branches->GetEntries();

   // std::vector<TMVAvar> values_variables(variables.size());
   // std::vector<TMVAvar> values_spectators(spectators.size());


   // TString Tvar_type;
   // for (unsigned i= 0; i < variables.size(); ++i) {
   //    TString variableName = variables[i];
   //    if(variableName == "selmu_pos"){
   //       Tvar_type = "F_arr4";
   //       values_variables[i].SetType(Tvar_type);
   //    }
   //    else{
   //       Tvar_type = inputTree->GetLeaf(variableName)->GetTypeName()[0];
   //       values_variables[i].SetType(Tvar_type);
   //    }
   //    values_variables[i].SetValueAddress(inputTree, variableName);
         
   // }
   // for (unsigned i= 0; i < spectators.size(); ++i) {
   //    TString variableName = spectators[i];
   //    var_type = inputTree->GetLeaf(variableName)->GetTypeName()[0];
   //    values_spectators[i].SetType(var_type);
   //    values_spectators[i].SetValueAddress(inputTree, variableName);
   //    //inputTree->SetBranchAddress(variableName, &values_spectators[i]);
   // }

   // Int_t p0dleadtopology;
   // inputTree->SetBranchAddress("p0dleadtopology", &p0dleadtopology);


   // std::vector<Float_t> values_variables_reader(n_vars);
   // std::vector<Float_t> values_spectators_reader(spectators.size());


   // unsigned int j = 0;
   // for (unsigned i = 0; i < variables.size(); ++i) {
   //    TString variable = variables[i];
   //    var_type = inputTree->GetLeaf(variable)->GetTypeName()[0];
   //    if(variable == "selmu_pos"){
   //       for(j; j < 3; ++j){
   //          TString index = std::to_string(j);
   //          TString var_str = variable + "[" + index + "]";
   //          reader->AddVariable(var_str, &values_variables_reader[i+j]);
   //       }
   //       j -=1;
   //    }
   //    else{
   //       reader->AddVariable(variables[i], &values_variables_reader[i+j]);
   //    }
      
   //    //reader->AddVariable("log("+variables[i]+")", &values_variables_reader[i]);
   // }
   // for (unsigned i = 0; i < spectators.size(); ++i) {
   //    // TString variableName = variables[i];
   //    // var_type = inputTree->GetLeaf(variableName)->GetTypeName()[0];
   //    // values_variables_reader[i].SetType(var_type);
   //    // values_variables_reader[i].AddVarToReader(reader, variableName);
   //    reader->AddSpectator(spectators[i], &values_spectators_reader[i]);
   //    //reader->AddSpectator("log("+variables[i]+")", &values_spectators_reader[i]);
   // }

   // std::cout<<"All variables added to Reader"<<"\n";

   // Double_t BDT_response; 
   // TString dir    = "dataset/weights/";
   // TString prefix = "TMVAClassification";
   // //TString methodName = "BDTG method";
   // //TString weightfile = dir + prefix + "_BDTG.weights.xml";
   // TString BDTG_weights = dir + prefix + "_BDTG.weights.xml";
   // TString PDERS_weights = dir + prefix + "_PDERS.weights.xml";
   // TString PDEFoam_weights = dir + prefix + "_PDEFoam.weights.xml";   



   // reader->BookMVA("BDTG method", BDTG_weights);
   // //reader->BookMVA("PDERS method", PDERS_weights);
   // //reader->BookMVA("PDEFoam method", PDEFoam_weights);
   
   // Int_t nEvent = (Int_t) inputTree->GetEntries();            //Here, you can decide if you want to run your programme on a full sample or just test it on 1000 events
   // //Int_t nEvent = 1000;

   // // TFile *newfile = new TFile(postBDT_file,"recreate");              //post BDT analysis file creation
   // // TTree *newtree = inputTree->CloneTree(0);                
   // // TBranch *myBDTr = newtree->Branch("BDT_response", &BDT_response);

   // std::vector<Float_t> out_vec(nEvent);
   // std::vector<Float_t> class_vec(nEvent);

   
   // for (Long64_t ievt=0; ievt<nEvent; ievt++) {
   //    inputTree->GetEntry(ievt);
   //    if(ievt % 10000 == 0) cout << " Processed: " << ievt << " entries " <<endl;
   //    j = 0;       
   //    for (unsigned int i=0; i<variables.size(); i++){ 
   //       if(variables[i] == "selmu_pos"){
   //          for (j; j < 3; ++j){
   //             values_variables_reader[i+j]=values_variables[i].GetNthValueAsFloat(j); 
   //          }
   //          j -= 1;
   //       }  
   //       else
   //          values_variables_reader[i+j]=values_variables[i].GetValueAsFloat(); 
   //    }
   //    for (unsigned int i=0; i<spectators.size(); i++)
   //          values_spectators_reader[i]=values_spectators[i].GetValueAsFloat();
   //    BDT_response = reader->EvaluateMVA("BDTG method");
   //    out_vec.at(ievt) = BDT_response;
   //    if(p0dleadtopology == 0)
   //       class_vec.at(ievt) = 1;
   //    else
   //       class_vec.at(ievt) = 0;

   //    //reader->EvaluateMVA("PDERS method");
   //    //reader->EvaluateMVA("PDEFoam method");
   //    //newtree->Fill();

   // }  
   // //newfile->Write();
   // //delete newfile;

   // std::cout<<"Loop over"<<std::endl;
   // TMVA::ROCCurve roc {out_vec, class_vec};


   // delete reader;

   // std::cout<<"Reader deleted"<<std::endl;
   // //Stops the clock's countdown
   // auto finish = std::chrono::high_resolution_clock::now();
   // std::chrono::duration<double> elapsed = finish - start;

   // //Final results 
   // std::cout<<"BDT  cut: "<<cut<<"\n";
   // std::cout << "Elapsed time: " << floor(elapsed.count()/60) << " min "<< fmod(elapsed.count(), 60) << " s\n";
   // std::cout<<"AUC / ROC integral for test data: "<<AUC<<"\n";
   // std::cout<<"AUC / ROC integral for input tree: "<<roc.GetROCIntegral()<<"\n";


   // if (!gROOT->IsBatch()) TMVA::TMVAGui( outfileName );  //Comment this line if you do not want to display the GUI

   // return AUC;

   return 0;
   
}


