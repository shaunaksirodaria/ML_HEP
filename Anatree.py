from ROOT import TFile, gDirectory
from ROOT import TH1D, TCanvas

myfile = TFile( 'experiment.root' )

mychain = gDirectory.Get( 'tree1' )
entries = mychain.GetEntriesFast()

chi2Hist = TH1D("chi2","Histogram of Chi2",100,0,20)

for jentry in xrange( entries ):
   ientry = mychain.LoadTree( jentry )
   if ientry < 0:
      break
   nb = mychain.GetEntry( jentry )
   if nb <= 0:
      continue
   chi2 = mychain.chi2
   chi2Hist.Fill(chi2)
   
canvas = TCanvas()
chi2Hist.Draw()
canvas.Draw()
