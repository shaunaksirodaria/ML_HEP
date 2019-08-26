#define hits_cxx
#include "hits.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TObject.h>
#include <TProfile.h>

void hits::Loop()
{
//   In a ROOT session, you can do:
//      root> .L hits.C
//      root> hits t
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

   TCanvas *p1=new TCanvas("p1","Longitudinal Profile",200,10,700,500);
   auto hprof = new TProfile("hprof","Longitudinal Profile",100,0,40,0.,6000.);

   TH1F *hR1[number_of_layers]; TH1F *hR17[number_of_layers]; 
   TH1F *hR7[number_of_layers]; TH1F *hR19[number_of_layers]; TH1F *hR719[number_of_layers]; 
   char *Layer_hR1=new char[number_of_layers];
   char *Layer_hR7=new char[number_of_layers];
   char *Layer_hR19=new char[number_of_layers];
   char *Layer_hR17=new char[number_of_layers];
   char *Layer_hR719=new char[number_of_layers];
   for(int j=0;j<number_of_layers;j++){
     sprintf(Layer_hRecx,"Layer_%d_hRecx",j);
     hRecx[j] = new TH2F(Layer_hRecx,Layer_hRecx,50,-9.0,9.0,50,-9.0,9.0);
     sprintf(Layer_hR1,"Layer_%d_hR1",j);
     hR1[j] = new TH1F(Layer_hR1,Layer_hR1,300,0.,3000.);   
     sprintf(Layer_hR7,"Layer_%d_hR7",j);
     hR7[j] = new TH1F(Layer_hR7,Layer_hR7,300,0.,3000.);
     sprintf(Layer_hR17,"Layer_%d_hR17",j);
     hR17[j] = new TH1F(Layer_hR17,Layer_hR17,300,0.,1.);
     sprintf(Layer_hR19,"Layer_%d_hR19",j);
     hR19[j] = new TH1F(Layer_hR19,Layer_hR19,300,0.,3000.);
     sprintf(Layer_hR719,"Layer_%d_hR719",j);
     hR719[j] = new TH1F(Layer_hR719,Layer_hR719,300,0.,1.);
   }
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();
   //nentries=10;
   Long64_t nbytes = 0, nb = 0;
   float  default_value = 0.;
   short uc=0;
   short vc=0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     Long64_t ientry = LoadTree(jentry);
     if (ientry < 0) break;
     nb = fChain->GetEntry(jentry);   nbytes += nb;
     std::vector<short> Recx(number_of_layers, default_value);


     for(unsigned int i=0;i<NRechits;i++){
       hRecx[rechit_layer->at(i)]->Fill(rechit_iu->at(i),rechit_iv->at(i), rechit_energy->at(i));
     }
  }   
  TFile *layerwise_R=new TFile("layerwise_R.root","recreate");
  
  for(int j=0;j<number_of_layers;j++){
    hRecx[j]->Write();
    
    TCanvas *a[j];
    a[j]=new TCanvas(Form("b%i",j),Form("b%i",j),800,1000);
    a[j]->cd();
    hRecx[j]->Draw("COLZ");
    Int_t MaxBin=hRecx[j]->GetMaximumBin();
    Int_t x,y,z;
    hRecx[j]->GetBinXYZ(MaxBin,x,y,z);
    short uc=hRecx[j]->GetXaxis()->GetBinCenter(x);
    short vc=hRecx[j]->GetYaxis()->GetBinCenter(y);
    printf("The bin in layer %d having the maximum value is (%d,%d) at (%i,%i)",j,x,y,uc,vc);    
    a[j]->SaveAs(Form("Layer_%i_hRecx.png",j));
  }

//second event loop for looping with new centres
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    std::vector<float> R1(number_of_layers, default_value);
    std::vector<float> R7(number_of_layers, default_value);
    std::vector<float> R17(number_of_layers, default_value);
    std::vector<float> R19(number_of_layers, default_value);
    std::vector<float> R719(number_of_layers, default_value);

    for(unsigned int i=0;i<NRechits;i++){
      if(abs(rechit_iu->at(i)-uc)<2 && abs(rechit_iv->at(i)-vc)<2){ 
        R7[rechit_layer->at(i)]+=rechit_energy->at(i);
      }
      if(rechit_iu->at(i)==0 && rechit_iv->at(i)==0){
        R1[rechit_layer->at(i)]+=rechit_energy->at(i);
      }
      if(abs(rechit_iu->at(i)-uc)<3 && abs(rechit_iv->at(i)-vc)<3){ 
        R19[rechit_layer->at(i)]+=rechit_energy->at(i);
      }
      hprof->Fill(rechit_layer->at(i),rechit_energy->at(i));
    }

    for(int j=0;j<number_of_layers;j++){    
      R17[j]=R1[j]/R7[j];
      R719[j]=R7[j]/R19[j];
      hR1[j]->Fill(R1[j]);
      hR19[j]->Fill(R19[j]);
      hR7[j]->Fill(R7[j]);
      hR719[j]->Fill(R719[j]);
      hR17[j]->Fill(R17[j]);
       
    }
  }
  hprof->Write();
  hprof->SaveAs("LongProf.png");
  for(int j=0;j<number_of_layers;j++){
    hR1[j]->Write();
    hR7[j]->Write();
    hR19[j]->Write();
    hR719[j]->Write();
    hR17[j]->Write();
  
    TCanvas *c[j];
    c[j]=new TCanvas(Form("c%i",j),Form("c%i",j),800,1000);
    c[j]->SetLogy(); c[j]->SetLogx();
    c[j]->cd();
    hR7[j]->Draw("E");
    c[j]->SaveAs(Form("Layer_%i_hR7.png",j));

    TCanvas *b[j];
    b[j]=new TCanvas(Form("b%i",j),Form("b%i",j),800,1000);
    b[j]->SetLogy(); b[j]->SetLogx();
    b[j]->cd();
    hR1[j]->Draw("E");
    b[j]->SaveAs(Form("Layer_%i_hR1.png",j));

    TCanvas *d[j];
    d[j]=new TCanvas(Form("d%i",j),Form("d%i",j),800,1000);
    d[j]->SetLogy(); d[j]->SetLogx();
    d[j]->cd();
    hR19[j]->Draw("E");
    d[j]->SaveAs(Form("Layer_%i_hR19.png",j));

    TCanvas *e[j];
    e[j]=new TCanvas(Form("e%i",j),Form("e%i",j),800,1000);
    e[j]->SetLogy(); e[j]->SetLogx();
    e[j]->cd();
    hR17[j]->Draw("E");
    e[j]->SaveAs(Form("Layer_%i_hR17.png",j));
 
    TCanvas *g[j];
    g[j]=new TCanvas(Form("g%i",j),Form("g%i",j),800,1000);
    g[j]->SetLogy(); g[j]->SetLogx();
    g[j]->cd();
    hR719[j]->Draw("E");
    g[j]->SaveAs(Form("Layer_%i_hR719.png",j));
  }
layerwise_R->Close();
}
