from ROOT import TFile, gDirectory, gROOT, TH1D
from ROOT import TH1F, TCanvas, gStyle

myfile1 = TFile('experiment.root')

mychain = gDirectory.Get( 'tree1' )
entries = mychain.GetEntriesFast()
myHistfit = TH1D("ebeam","ebeam cut",100,149.2,150.6)
gStyle.SetOptFit(1112)
chi2Hist = TH1D("chi2","Histogram of Chi2",100,0,20)
gStyle.SetOptFit(0100)

for jentry in xrange( entries ):
   ientry = mychain.LoadTree( jentry )
   if ientry < 0:
      break
   nb = mychain.GetEntry( jentry )
   if nb <= 0:
      continue
   myValue = mychain.ebeam
   if(149.8<myValue<150.2):	
    myHistfit.Fill(myValue)
    myHistfit.Fit("gaus")
   chi2 = mychain.chi2
   chi2Hist.Fill(chi2)

c1 = TCanvas()
c1.cd()
chi2Hist.Draw()
c1.Draw()
c2 = TCanvas()
c2.cd()
myHistfit.Draw()
c2.Draw()
c1.SaveAs("chi2Hist.png")
c2.SaveAs("myHistfit.png")