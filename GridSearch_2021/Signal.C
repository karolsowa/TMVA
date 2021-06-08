#include <TROOT.h>
#include <TChain.h>
#include "TH1F.h"
#include <TFile.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <string>
#include "/home/sowrol/lhcbStyle.c"


void Signal(bool mode = 1){
	//gROOT->Reset();
    //gROOT->ProcessLine(".L /home/sowrol/lhcbStyle.c");
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
    TH1D* B_all;
    TH1D* D_all;
    TH1D* K_all;

    const Double_t BDT_GS = 0.69869;//0.42683;//
    const Double_t BDT_HPO = 0.210812;//0.288832;//
    const Double_t BDT_OTP = 0.0884549;
    

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

    
    LHCbStyle->SetTitleSize(0.02,"x");
    // styleptr->SetTitleSize(0.02,"y");
    // styleptr->SetTitleSize(0.02,"z");
    LHCbStyle->SetLabelSize(0.02,"x");
    // styleptr->SetLabelSize(0.02,"y");
    // styleptr->SetLabelSize(0.02,"z");

    gROOT->SetStyle("LHCbStyle");
    gROOT->ForceStyle();

    B_GS = new TH1D("B_GS","B_GS" , B_NBINS, min_Bmass, max_Bmass ); 
    B_HPO = new TH1D("B_HPO","B_HPO" , B_NBINS, min_Bmass, max_Bmass );
    B_OTP = new TH1D("B_OTP","B_OTP" , B_NBINS, min_Bmass, max_Bmass );
    B_all = new TH1D("B_all","B_all" , B_NBINS, min_Bmass, max_Bmass ); 

    D_GS = new TH1D("Ds_GS" , "Ds_GS" ,D_NBINS, min_D0mass, max_D0mass);
    D_HPO = new TH1D("Ds_HPO" , "Ds_HPO" ,D_NBINS, min_D0mass, max_D0mass);
    D_OTP = new TH1D("Ds_OTP" , "Ds_OTP" ,D_NBINS, min_D0mass, max_D0mass);
    D_all = new TH1D("Ds_all" , "Ds_all" ,D_NBINS, min_D0mass, max_D0mass);

    K_GS = new TH1D("K_GS" , "K_GS" ,K_NBINS, min_Ks0mass, max_Ks0mass);
    K_HPO = new TH1D("K_HPO" , "K_HPO" ,K_NBINS, min_Ks0mass, max_Ks0mass);
    K_OTP = new TH1D("K_OTP" , "K_OTP" ,K_NBINS, min_Ks0mass, max_Ks0mass);
    K_all = new TH1D("K_all" , "K_all" ,K_NBINS, min_Ks0mass, max_Ks0mass);


    while (GS_fReader->Next()){
        B_all->Fill(*GS_B_M);
        D_all->Fill(*GS_D0_M);
        K_all->Fill(*GS_KS0_M);
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
            K_OTP->Fill(*OTP_KS0_M);  
   		}
    }


    TCanvas* B_can = new TCanvas("B_can", "B_can", 0, 0, 800, 600);
    //gPad->SetRightMargin(0.05);
    //gPad->SetLeftMargin(0.15);
    gStyle->SetOptStat(0);
    B_all->SetLineColor(kBlack);
    B_GS->SetLineColor(kBlue);
    B_HPO->SetLineColor(kRed);
    B_OTP->SetLineColor(kGreen);
    B_all->SetXTitle("Mass [MeV/c^{2}]");
    B_all->SetYTitle("Events");
    B_all->SetTitle("B mass");
    gStyle->SetLabelSize(0.1,"x");
    //B_all->GetYaxis()->SetTitleOffset(1.5);

    // if (mode)
    //     B_HPO->GetYaxis()->SetRangeUser(0,50000);
    auto B_legend = new TLegend(0.75,0.7,0.9,0.9);
    B_all->Draw();
    B_HPO->Draw("SameHist");
    B_GS->Draw("SameHist");
    //if (mode)
    B_OTP->Draw("SameHist");
    //LHCbStyle::lhcbName->Draw("same");

    B_legend->AddEntry(B_all, "Data", "l");
    B_legend->AddEntry(B_GS, "GridSearch ", "l");
    B_legend->AddEntry(B_HPO, "HPO ", "l");
    //if (mode)
    B_legend->AddEntry(B_OTP, "OTP ", "l");
    B_legend->Draw();

    TCanvas* D_can = new TCanvas("D_can", "D_can", 0, 0, 800, 600);
    //gPad->SetRightMargin(0.05);
    //gPad->SetLeftMargin(0.15);
    gStyle->SetOptStat(0);
    D_all->SetLineColor(kBlack);
    D_GS->SetLineColor(kBlue);
    D_HPO->SetLineColor(kRed);
    D_OTP->SetLineColor(kGreen);
    D_all->SetXTitle("Mass [MeV/c^{2}]");
    D_all->SetYTitle("Events");
    D_all->SetTitle("D0 mass");
    //D_all->GetYaxis()->SetTitleOffset(1.5);
    auto D_legend = new TLegend(0.75,0.7,0.9,0.9);
    // if (mode)
    //     D_HPO->GetYaxis()->SetRangeUser(0,50000);
    D_all->Draw();
    D_HPO->Draw("SameHist");
    D_GS->Draw("SameHist");
    //if (mode)
    D_OTP->Draw("SameHist");

    D_legend->AddEntry(D_all, "Data", "l");
    D_legend->AddEntry(D_GS, "GridSearch ", "l");
    D_legend->AddEntry(D_HPO, "HPO ", "l");
    //if (mode)
    D_legend->AddEntry(D_OTP, "OTP ", "l");
    D_legend->Draw();

    TCanvas* K_can = new TCanvas("K_can", "K_can", 0, 0, 800, 600);
    //gPad->SetRightMargin(0.05);
    //gPad->SetLeftMargin(0.15);
    gStyle->SetOptStat(0);
    K_all->SetLineColor(kBlack);
    K_GS->SetLineColor(kBlue);
    K_HPO->SetLineColor(kRed);
    K_OTP->SetLineColor(kGreen);
    K_all->SetXTitle("Mass [MeV/c^{2}]");
    K_all->SetYTitle("Events");
    K_all->SetTitle("KS0 mass");
    //K_all->GetYaxis()->SetTitleOffset(1.5);
    auto K_legend = new TLegend(0.75,0.7,0.9,0.9);
    // if (mode)
        // K_HPO->GetYaxis()->SetRangeUser(0,50000);
    K_all->Draw();
    K_HPO->Draw("SameHist");
    K_GS->Draw("SameHist");
    //if (mode)
    K_OTP->Draw("SameHist");

    K_legend->AddEntry(K_all, "Data", "l");
    K_legend->AddEntry(K_GS, "GridSearch ", "l");
    K_legend->AddEntry(K_HPO, "HPO ", "l");
    //if (mode)
    K_legend->AddEntry(K_OTP, "OTP ", "l");
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