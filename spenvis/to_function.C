TGraph *graph = nullptr;

void to_function()
{
  gStyle -> SetOptStat(0);

  /// input data
  graph = new TGraph();
  graph -> SetMarkerStyle(20);
  graph -> SetMarkerSize(0.3);

  ifstream input("h_flux.dat");
  Double_t x, y1, y2;
  while (input >> x >> y1 >> y2)
    graph -> SetPoint(graph->GetN(),x,y2);

  /// function using inter/extra-polation of the graph
  TF1 *pdf = new TF1("pdf",[&](double*x, double*){ return graph->Eval(x[0]); }, 1, 5000, 0);
  pdf -> SetNpx(1000);

  /// confirmation
  auto hist = new TH1D("hist",";Energy [MeV/n];Flux of H [m^{-1}sr^{-1}s^{-1}]",400,1,5000);
  for (auto i=0; i<1000000; ++i)
    hist -> Fill(pdf->GetRandom());
  hist -> Scale(1.24/hist -> GetBinContent(hist->GetMaximumBin()));

  /// draw
  auto cvs = new TCanvas("cvs","",800,500);
  hist -> SetMaximum(1.5);
  hist -> Draw("hist");
  graph -> Draw("samep");
  pdf -> Draw("samel");

  //cvs -> SetLogx();
  //cvs -> SetLogy();
}
