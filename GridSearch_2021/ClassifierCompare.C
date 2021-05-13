#include <TROOT.h>
#include <TChain.h>
#include "TH1F.h"
#include <TFile.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <string>



void ClassifierCompare(bool mode = 1){
	gROOT->Reset();
	TString GS_filename = "/home/sowrol/repo/MyBDT_cascade1_job2_GS.root";
	TString HPO_filename = "/home/sowrol/repo/MyBDT_cascade1_job2_HPO.root";
	TString OTP_filename = "/home/sowrol/repo/MyBDT_cascade1_job2_OTP.root";
	TFile *GS_File = TFile::Open(GS_filename);
	TFile *HPO_File = TFile::Open(HPO_filename);
	TFile *OTP_File = TFile::Open(OTP_filename);
	TTreeReader *GS_fReader = new TTreeReader("DecayTree", GS_File);  //!the tree reader
	TTreeReader *HPO_fReader = new TTreeReader("DecayTree", HPO_File);  //!the tree reader
	TTreeReader *OTP_fReader = new TTreeReader("DecayTree", OTP_File);  //!the tree reader
    //TTreeReaderValue<Double_t> B_M(fReader, "B_M");
    TTreeReaderValue<Double_t> GS_B_M(*GS_fReader, "B_M");
    TTreeReaderValue<Double_t> GS_D0_M(*GS_fReader, "D0_M");
    TTreeReaderValue<Double_t> GS_BDT_response(*GS_fReader, "BDT_response");
    TTreeReaderValue<Double_t> HPO_B_M(*HPO_fReader, "B_M");
    TTreeReaderValue<Double_t> HPO_D0_M(*HPO_fReader, "D0_M");
    TTreeReaderValue<Double_t> HPO_BDT_response(*HPO_fReader, "BDT_response");
    TTreeReaderValue<Double_t> OTP_B_M(*OTP_fReader, "B_M");
    TTreeReaderValue<Double_t> OTP_D0_M(*OTP_fReader, "D0_M");
    TTreeReaderValue<Double_t> OTP_BDT_response(*OTP_fReader, "BDT_response");

    TH1D* B_GS;
    TH1D* B_HPO;
    TH1D* B_OTP;
    TH1D* D_GS;    
    TH1D* D_HPO;
    TH1D* D_OTP;

    const Double_t BDT_GS = 0.69869;
    const Double_t BDT_HPO = 0.210812;
    const Double_t BDT_OTP = 0.0884549;
    

    Double_t min_Bmass = 4600;
   	Double_t max_Bmass = 6100; //5.6
    Double_t min_D0mass = 1810; // 1.820;  
    Double_t max_D0mass =1950; // 2.02;

    Int_t B_NBINS = 80;
    Int_t D_NBINS = 100;

    B_GS = new TH1D("B_GS","B_OTP" , B_NBINS, min_Bmass, max_Bmass ); 
    B_HPO = new TH1D("B_HPO","B_HPO" , B_NBINS, min_Bmass, max_Bmass );
    B_OTP = new TH1D("B_OTP","B_OTP" , B_NBINS, min_Bmass, max_Bmass );

    D_GS = new TH1D("Ds_GS" , "Ds_GS" ,D_NBINS, min_D0mass, max_D0mass);
    D_HPO = new TH1D("Ds_HPO" , "Ds_HPO" ,D_NBINS, min_D0mass, max_D0mass);
    D_OTP = new TH1D("Ds_OTP" , "Ds_OTP" ,D_NBINS, min_D0mass, max_D0mass);


    while (GS_fReader->Next()){
   		if(*GS_BDT_response > BDT_GS){
        	B_GS->Fill(*GS_B_M);
        	D_GS->Fill(*GS_D0_M);
   		}
    }

    while (HPO_fReader->Next()){
    	if(*HPO_BDT_response > BDT_HPO){
        	B_HPO->Fill(*HPO_B_M);
        	D_HPO->Fill(*HPO_D0_M);
   		}
    }

    while (OTP_fReader->Next()){
    	if(*OTP_BDT_response > BDT_OTP){
        	B_OTP->Fill(*OTP_B_M);
        	D_OTP->Fill(*OTP_D0_M);
   		}
    }


    TCanvas* B_can = new TCanvas("B_can", "B_can", 0, 0, 800, 600);
    gPad->SetRightMargin(0.05);
    gPad->SetLeftMargin(0.15);
    gStyle->SetOptStat(0);
    B_GS->SetLineColor(kBlue);
    B_HPO->SetLineColor(kRed);
    B_OTP->SetLineColor(kGreen);

    if (mode)
        B_HPO->GetYaxis()->SetRangeUser(0,50000);
    auto B_legend = new TLegend(0.8,0.7,0.95,0.9);

    B_HPO->Draw();
    B_GS->Draw("SameHist");
    if (mode)
        B_OTP->Draw("SameHist");

    B_legend->AddEntry(B_GS, "GridSearch ", "l");
    B_legend->AddEntry(B_HPO, "HPO ", "l");
    if (mode)
        B_legend->AddEntry(B_OTP, "OTP ", "l");
    B_legend->Draw();
    
    

    TCanvas* D_can = new TCanvas("D_can", "D_can", 0, 0, 800, 600);
    gPad->SetRightMargin(0.05);
    gPad->SetLeftMargin(0.15);
    gStyle->SetOptStat(0);
    D_GS->SetLineColor(kBlue);
    D_HPO->SetLineColor(kRed);
    D_OTP->SetLineColor(kGreen);
    auto D_legend = new TLegend(0.8,0.7,0.95,0.9);
    if (mode)
        D_HPO->GetYaxis()->SetRangeUser(0,50000);
    D_HPO->Draw();
    D_GS->Draw("SameHist");
    if (mode)
        D_OTP->Draw("SameHist");

    D_legend->AddEntry(D_GS, "GridSearch ", "l");
    D_legend->AddEntry(D_HPO, "HPO ", "l");
    if (mode)
        D_legend->AddEntry(D_OTP, "OTP ", "l");
    D_legend->Draw();
    
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