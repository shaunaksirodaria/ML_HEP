#define Ana_cxx
#include "Ana.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>

using namespace std;
void Ana::Loop()
{
//   In a ROOT session, you can do:
//      root> .L Ana.C
//      root> Ana t
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
   TH1F *hR7=new TH1F("hR7","R7",300,0.,300000.);

   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();
   nentries=5 ;
   Long64_t nbytes = 0, nb = 0;
   int number_of_layers = 38;
   float  default_value = 0;

   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     Long64_t ientry = LoadTree(jentry);
     if (ientry < 0) break;
     nb = fChain->GetEntry(jentry);   nbytes += nb;
     std::vector<float> R7(number_of_layers, default_value);
     for(unsigned int i=0;i<NRechits;i++){
       if(abs(rechit_iu->at(i))<2 && abs(rechit_iv->at(i))<2 && (rechit_iu->at(i)*rechit_iv->at(i))>=0){ 
       std::cout<<"NRechits:"<<NRechits<<std::endl;
       std::cout<<"rechit_iu:rechit_iv"<<rechit_iu->at(i)<<":"<<rechit_iv->at(i)<<std::endl;
       std::cout<<"rechit_layer:"<<rechit_layer->at(i)<<std::endl;
       R7[rechit_layer->at(i)]+=rechit_energy->at(i);
      }
      std::cout<<"R7:"<<R7[rechit_layer->at(i)]<<std::endl;
      hR7->Fill(R7[rechit_layer->at(i)]);
      hR7->Draw();
     }
  }
  
  TFile *layerwise_selection=new TFile("layerwise_selection.root","recreate");
  hR7->Write();
  layerwise_selection->Close();
}
