void read()
{
  auto file = new TFile("human_phantom.root");
  file -> ls();

  auto dir = file -> GetDirectory("human_phantom_ntuple");
  dir -> ls();

  auto tree = (TTree *) dir -> Get("data");

  auto hist25 = new TH1D("hist25",";edep",100,0,100);
  hist25 -> SetLineColor(kRed);

  auto hist26 = new TH1D("hist26",";edep",100,0,100);
  hist26 -> SetLineColor(kBlue);

  new TCanvas();
  tree -> Draw("edep>>hist26","organID==26");
  tree -> Draw("edep>>hist25","organID==25","same");

  auto hist2D = new TH2D("hist2D",";edep",200,0,4000,200,0,100);

  new TCanvas();
  tree -> Draw("edep:primary_energy>>hist2D","organID==25","colz");
}
