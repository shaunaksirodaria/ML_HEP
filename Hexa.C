#define Hexa_cxx
#include "Hexa.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>

using namespace std;

void Hexa::Loop()
{
//   In a ROOT session, you can do:
//      root> .L Hexa.C
//      root> Hexa t
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

  TH1F *h_R7=new TH1F("h_R7","R7",100,0.,6000.);
  TH1F *h_R719=new TH1F("h_R719","R719",100,0.,6000.);

   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<1;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      float totRechitE=0.;float R7=0.;float R719=0.;
      for(int i=0;i<NRechits;i++){
	for(int u=-1;u<2;u++){
	  for(int v=-1;v<2;v++){
	    
	    R7+=rechit_energy->at(i);
	  }
        }
      }

      for(int i=0;i<NRechits;i++){
	for(int u=-2;u<3;u++){
	  for(int v=-2;v<3;v++){
	   
	    R719+=rechit_energy->at(i);
	  }
        }
      }

      std::cout<<"NRechits: "<<NRechits<<std::endl;
      std::cout<<"R7: "<<R7<<std::endl;
      std::cout<<"R719: "<<R719<<::endl;
   }
}
