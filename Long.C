#define Long_cxx
#include "Long.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TObject.h>
#include <TProfile.h>

void Long::Loop()
{
//   In a ROOT session, you can do:
//      root> .L Long.C
//      root> Long t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch

   
   int number_of_layers = 41;float default_value=0.;
   
   TCanvas *c1=new TCanvas("c1","Longitudinal Profile",200,10,700,500);
   auto hprof = new TProfile("hprof","Longitudinal Profile",100,0,40,0.,6000.);
   std::vector<float> Longp(number_of_layers, default_value);
   int mLongp=0.;
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();
   // nentries=100;
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
     
      for(unsigned int i=0;i<NRechits;i++){
	//	Longp[rechit_layer->at(i)]=Fillrechit_energy->at(i);
	//	mLongp=Longp[rechit_layer->at(i)]/rechit_layer->size();
      	hprof->Fill(rechit_layer->at(i),rechit_energy->at(i));
      }	
      hprof->Draw();
      hprof->SaveAs("LongProf.png");
   }
}
