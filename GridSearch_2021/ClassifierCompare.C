#include <TROOT.h>
#include <TChain.h>
#include "TH1F.h"
#include <TFile.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <string>



void ClassifierCompare(std::string filename){
	gROOT->Reset();
	TFile *myFile = TFile::Open(filename.c_str());
	TTreeReader *fReader = new TTreeReader("DecayTree", myFile);  //!the tree reader
    //TTreeReaderValue<Double_t> B_M(fReader, "B_M");
    TTreeReaderValue<Double_t> B_M(*fReader, "B_M");
    TTreeReaderValue<Double_t> D0_M(*fReader, "D0_M");
    TTreeReaderValue<Double_t> BDT_response(*fReader, "BDT_response");

    TH1D* B_HPO;
    TH1D* B_OTP;
    TH1D* B_GS;
    TH1D* D_HPO;
    TH1D* D_OTP;
    TH1D* D_GS;
    const Double_t BDT_HPO = 0.074918;
    const Double_t BDT_OTP = 0.0884549;
    const Double_t BDT_GS = 0.33856;

    Double_t min_Bmass = 4600;
   	Double_t max_Bmass = 6100; //5.6
    Double_t min_D0mass = 1750; // 1.820;  
    Double_t max_D0mass =1950; // 2.02;

    Int_t B_NBINS = 80;
    Int_t D_NBINS = 100;

    B_HPO = new TH1D("B_HPO","B_HPO" , B_NBINS, min_Bmass, max_Bmass );
    B_OTP = new TH1D("B_OTP","B_OTP" , B_NBINS, min_Bmass, max_Bmass );
    B_GS = new TH1D("B_GS","B_OTP" , B_NBINS, min_Bmass, max_Bmass ); 

    D_HPO = new TH1D("Ds_HPO" , "Ds_HPO" ,D_NBINS, min_D0mass, max_D0mass);
    D_OTP = new TH1D("Ds_OTP" , "Ds_OTP" ,D_NBINS, min_D0mass, max_D0mass);
    D_GS = new TH1D("Ds_GS" , "Ds_GS" ,D_NBINS, min_D0mass, max_D0mass);

    while (fReader->Next()){
    	if(*BDT_response > BDT_HPO){
        	B_HPO->Fill(*B_M);
        	D_HPO->Fill(*D0_M);
   		}
   		if(*BDT_response > BDT_OTP){
        	B_OTP->Fill(*B_M);
        	D_OTP->Fill(*D0_M);
   		}
   		if(*BDT_response > BDT_GS){
        	B_GS->Fill(*B_M);
        	D_GS->Fill(*D0_M);
   		}
    }


    TCanvas* B_can = new TCanvas("B_can", "B_can", 0, 0, 800, 600);
    gPad->SetRightMargin(0.05);
    gPad->SetLeftMargin(0.15);
    B_HPO->Draw();
    B_OTP->Draw();
    B_GS->Draw();
    //myFile->Close();
    //delete fReader;
    //fReader = 0;

    std::cout<<"All done"<<"\n";
    //delete myFile;
    // delete B_HPO;
    // delete B_OTP;
    // delete B_GS;
    // delete D_HPO;
    // delete D_OTP;
    // delete D_GS;



}