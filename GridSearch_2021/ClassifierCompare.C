#include <TROOT.h>
#include <TChain.h>
#include "TH1F.h"
#include <TFile.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <string>
#include "/home/sowrol/lhcbStyle.c"

void ClassifierCompare(bool mode = 1){
	//gROOT->Reset();
    // gROOT->ProcessLine(".L lhcbstyle.C");
    // lhcbStyle();
    TStyle *LHCbStyle= new TStyle("LHCbStyle","LHCb plots style");
    lhcbStyle(LHCbStyle);
	TString GS_filename = "/data1/DATA_SOWA/BD_classifier/MyBDT_cascade1_job2_GS.root";
	TString HPO_filename = "/data1/DATA_SOWA/BD_classifier/MyBDT_cascade1_job2_HPO.root";
	TString OTP_filename = "/data1/DATA_SOWA/BD_classifier/MyBDT_cascade1_job2_OTP.root";
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
    TTreeReaderValue<Double_t> GS_KS0_M(*GS_fReader, "KS0_M");
    TTreeReaderValue<Double_t> HPO_B_M(*HPO_fReader, "B_M");
    TTreeReaderValue<Double_t> HPO_D0_M(*HPO_fReader, "D0_M");
    TTreeReaderValue<Double_t> HPO_BDT_response(*HPO_fReader, "BDT_response");
    TTreeReaderValue<Double_t> HPO_KS0_M(*HPO_fReader, "KS0_M");
    TTreeReaderValue<Double_t> OTP_B_M(*OTP_fReader, "B_M");
    TTreeReaderValue<Double_t> OTP_D0_M(*OTP_fReader, "D0_M");
    TTreeReaderValue<Double_t> OTP_BDT_response(*OTP_fReader, "BDT_response");
    TTreeReaderValue<Double_t> OTP_KS0_M(*OTP_fReader, "KS0_M");

    TH1D* B_GS;
    TH1D* B_HPO;
    TH1D* B_OTP;
    TH1D* D_GS;    
    TH1D* D_HPO;
    TH1D* D_OTP;
    TH1D* K_GS;
    TH1D* K_HPO;
    TH1D* KS0_OTP; 

    const Double_t BDT_GS = 0.42683;//0.69869;
    const Double_t BDT_HPO = 0.288832;//0.210812;
    //const Double_t BDT_OTP = 0.0884549;
    

    Double_t min_Bmass = 4600;
   	Double_t max_Bmass = 6100; //5.6
    Double_t min_D0mass = 1810; // 1.820;  
    Double_t max_D0mass =1950; // 2.02;
    Double_t min_Ks0mass = 450;
    Double_t max_Ks0mass = 550;
    Double_t min_Kstmass = 400;
    Double_t max_Kstmass = 4500;

    Int_t B_NBINS = 80;
    Int_t D_NBINS = 100;
    Int_t K_NBINS = 50;

    B_GS = new TH1D("B_GS","B_OTP" , B_NBINS, min_Bmass, max_Bmass ); 
    B_HPO = new TH1D("B_HPO","B_HPO" , B_NBINS, min_Bmass, max_Bmass );
    B_OTP = new TH1D("B_OTP","B_OTP" , B_NBINS, min_Bmass, max_Bmass );

    D_GS = new TH1D("Ds_GS" , "Ds_GS" ,D_NBINS, min_D0mass, max_D0mass);
    D_HPO = new TH1D("Ds_HPO" , "Ds_HPO" ,D_NBINS, min_D0mass, max_D0mass);
    D_OTP = new TH1D("Ds_OTP" , "Ds_OTP" ,D_NBINS, min_D0mass, max_D0mass);

    K_GS = new TH1D("K_GS" , "K_GS" ,K_NBINS, min_Ks0mass, max_Ks0mass);
    K_HPO = new TH1D("K_HPO" , "K_HPO" ,K_NBINS, min_Ks0mass, max_Ks0mass);
    K_OTP = new TH1D("K_OTP" , "K_OTP" ,K_NBINS, min_Ks0mass, max_Ks0mass);


    while (GS_fReader->Next()){
   		if(*GS_BDT_response > BDT_GS){
        	B_GS->Fill(*GS_B_M);
        	D_GS->Fill(*GS_D0_M);
            K_GS->Fill(*GS_KS0_M);
   		}
    }

    while (HPO_fReader->Next()){
    	if(*HPO_BDT_response > BDT_HPO){
        	B_HPO->Fill(*HPO_B_M);
        	D_HPO->Fill(*HPO_D0_M);
            K_HPO->Fill(*HPO_KS0_M);
   		}
    }

    while (OTP_fReader->Next()){
    	if(*OTP_BDT_response > BDT_OTP){
        	B_OTP->Fill(*OTP_B_M);
        	D_OTP->Fill(*OTP_D0_M);
         K_OTP->Fill(*OTP_KSO_M);  
   		}
    }


    TCanvas* B_can = new TCanvas("B_can", "B_can", 0, 0, 800, 600);
    gPad->SetRightMargin(0.05);
    gPad->SetLeftMargin(0.15);
    gStyle->SetOptStat(0);
    B_GS->SetLineColor(kBlue);
    B_HPO->SetLineColor(kRed);
    B_OTP->SetLineColor(kGreen);
    B_HPO->SetXTitle("Mass [MeV/c^2]");
    B_HPO->SetYTitle("Events");
    B_HPO->SetTitle("B mass");
    //B_HPO->GetYaxis()->SetTitleOffset(1.5);

    //if (mode)
    //    B_HPO->GetYaxis()->SetRangeUser(0,50000);
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
    LHCbStyle::lhcbName->Draw("same");

    TCanvas* D_can = new TCanvas("D_can", "D_can", 0, 0, 800, 600);
    gPad->SetRightMargin(0.05);
    gPad->SetLeftMargin(0.15);
    gStyle->SetOptStat(0);
    D_GS->SetLineColor(kBlue);
    D_HPO->SetLineColor(kRed);
    D_OTP->SetLineColor(kGreen);
    D_HPO->SetXTitle("Mass [MeV/c^2]");
    D_HPO->SetYTitle("Events");
    D_HPO->SetTitle("D0 mass");
    //D_HPO->GetYaxis()->SetTitleOffset(1.5);
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

    TCanvas* K_can = new TCanvas("K_can", "K_can", 0, 0, 800, 600);
    gPad->SetRightMargin(0.05);
    gPad->SetLeftMargin(0.15);
    gStyle->SetOptStat(0);
    K_GS->SetLineColor(kBlue);
    K_HPO->SetLineColor(kRed);
    D_OTP->SetLineColor(kGreen);
    K_HPO->SetXTitle("Mass [MeV/c^2]");
    K_HPO->SetYTitle("Events");
    K_HPO->SetTitle("KS0 mass");
    //K_HPO->GetYaxis()->SetTitleOffset(1.5);
    auto K_legend = new TLegend(0.8,0.7,0.95,0.9);
    if (mode)
        K_HPO->GetYaxis()->SetRangeUser(0,50000);
    K_HPO->Draw();
    K_GS->Draw("SameHist");
    if (mode)
        D_OTP->Draw("SameHist");

    K_legend->AddEntry(K_GS, "GridSearch ", "l");
    K_legend->AddEntry(K_HPO, "HPO ", "l");
    if (mode)
        D_legend->AddEntry(D_OTP, "OTP ", "l");
    K_legend->Draw();
    
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