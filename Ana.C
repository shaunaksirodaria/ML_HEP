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
  

 
  TH1F *hR7[42]; TH1F *hR1[42]; TH1F *hR17[42];
   char *Layer_hR7=new char[41];
   char *Layer_hR1=new char[41];
   char *Layer_hR17=new char[41];
   for(int j=0;j<41;j++){
     sprintf(Layer_hR7,"Layer_hR7_%d",j);
     hR7[j] = new TH1F(Layer_hR7,Layer_hR7,300,0.,300000.);
     sprintf(Layer_hR1,"Layer_hR1_%d",j);
     hR1[j] = new TH1F(Layer_hR1,Layer_hR1,300,0.,300000.);
     sprintf(Layer_hR17,"Layer_hR17_%d",j);
     hR17[j] = new TH1F(Layer_hR17,Layer_hR17,300,0.,1.);
   }
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();
   nentries=5;
   Long64_t nbytes = 0, nb = 0;
   int number_of_layers = 41;
   float  default_value = 0.;

   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     Long64_t ientry = LoadTree(jentry);
     if (ientry < 0) break;
     nb = fChain->GetEntry(jentry);   nbytes += nb;
     
     std::vector<float> R7(number_of_layers, default_value);
     std::vector<float> R1(number_of_layers, default_value);
     std::vector<float> R17(number_of_layers, default_value);

     for(unsigned int i=0;i<NRechits;i++){
       if(abs(rechit_iu->at(i))<2 && abs(rechit_iv->at(i))<2 && (rechit_iu->at(i)*rechit_iv->at(i))>=0){ 
	R7[rechit_layer->at(i)]+=rechit_energy->at(i);
       }
       if((rechit_iu->at(i))==0 && (rechit_iv->at(i))==0){ 
        R1[rechit_layer->at(i)]+=rechit_energy->at(i);
       }
       R17[rechit_layer->at(i)]=R1[rechit_layer->at(i)]/R7[rechit_layer->at(i)];
     }
     std::cout<<"Event number = "<<jentry+1<<std::endl;
     for(int j=0;j<41;j++){
       std::cout<<"for layer: "<<j+1<<", R1 = "<<R1[j]<<" , R7 = "<<R7[j]<<" and R17 = "<<R17[j]<<std::endl;
       hR1[j]->Fill(R1[j]);
       hR7[j]->Fill(R7[j]);
       hR17[j]->Fill(R17[j]);
     }
  }  
  TFile *layerwise_selection=new TFile("layerwise_selection.root","recreate");
  for(int j=0;j<41;j++){
    hR7[j]->Write();
    hR1[j]->Write();
    hR17[j]->Write();
  }
  layerwise_selection->Close();
}
