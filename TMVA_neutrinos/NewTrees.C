#include "TTree.h"
#include "TFile.h"

void NewTrees(){

	TFile* f = new TFile("Prod6BNeut2w.root"); 
	TTree* oldtree = ((TTree*)f->Get("default"));

	const auto nentries = oldtree->GetEntries();
	TFile* signal_f = new TFile("Signal.root", "recreate");
	
    auto signal_tree = oldtree->CloneTree(0);
    

    int p0dleadtopology;

    oldtree->SetBranchAddress("p0dleadtopology", &p0dleadtopology);

    for (int i = 0; i < nentries; i++) {
      oldtree->GetEntry(i);
      if (p0dleadtopology == 0)
        signal_tree->Fill();
   }

   signal_f->Write();



   TFile* bkg_f = new TFile("Background.root", "recreate");
   auto bkg_tree = oldtree->CloneTree(0);

   for (int i = 0; i < nentries; i++) {
      oldtree->GetEntry(i);
      if (p0dleadtopology != 0)
        bkg_tree->Fill();
   }

   bkg_f->Write();


}