#define Recx_cxx
#include "Recx.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void Recx::Loop()
{
//   In a ROOT session, you can do:
//      root> .L Recx.C
//      root> Recx t
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

   int number_of_layers = 41;
   TH2F *hRecx[number_of_layers];
   char *Layer_hRecx=new char[number_of_layers];
   for(int j=0;j<number_of_layers;j++){
     sprintf(Layer_hRecx,"Layer_%d_hRecx",j);
     hRecx[j] = new TH2F(Layer_hRecx,Layer_hRecx,50,-9.0,9.0,50,-9.0,9.0);
   }

   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();
   // nentries=50;
   Long64_t nbytes = 0, nb = 0;
   float  default_value = 0.;
   for (Long64_t jentry=0; jentry<nentries;jentry++) { 
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      
      std::vector<float> Recx(number_of_layers, default_value);
      
      for(unsigned int i=0;i<NRechits;i++) {
	  hRecx[rechit_layer->at(i)]->Fill(rechit_x->at(i),rechit_y->at(i), rechit_energy->at(i));
      }
    }  
    TFile *layerwise_position=TFile::Open("layerwise_position.root","recreate");
    for(int j=0;j<number_of_layers;j++){
      hRecx[j]->Write();
      TCanvas *c[j];
      c[j]=new TCanvas(Form("c%i",j),Form("c%i",j),800,1000);
      c[j]->cd();
      hRecx[j]->Draw("COLZ");
      c[j]->SaveAs(Form("Layer_%i_hRecx.png",j));
    }
    layerwise_position->Close();
}
