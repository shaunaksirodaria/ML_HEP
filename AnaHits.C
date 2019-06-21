#define AnaHits_cxx
#include "AnaHits.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>

using namespace std;

void AnaHits::Loop()
{
//   In a ROOT session, you can do:
//      root> .L AnaHits.C
//      root> AnaHits t
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
   
   TH1F *h_totRechitE = new TH1F("h_totRechitE","Total Rechit Energy",100,0.,6000.);

   if (fChain == 0) return;
   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      float totRechitE=0.;
      for (unsigned int i=0; i<NRechits; i++){      
	totRechitE+=rechit_energy->at(i);
      }
      h_totRechitE->Fill(totRechitE);

      std::cout<<"NRechits"<<NRechits<<std::endl;
      std::cout<<"rechit_energy size"<<rechit_energy->size()<<std::endl;
      std::cout<<"totRechitE:"<<totRechitE<<std::endl;
   }
   h_totRechitE->Draw();
}
