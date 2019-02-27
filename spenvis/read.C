#include "style.h"
using namespace style;

void read()
{
  gStyle -> SetOptStat(0);

  TGraph *graph[8];
  for (auto i=0; i<8; ++i) {
    graph[i] = new TGraph();
    graph[i] -> SetLineColor(i+1);
  }

  ifstream input("flux.dat");
  Double_t x, dummy, fl[8];
  while (input >> x >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> fl[0] >> fl[1] >> fl[2] >> fl[3] >> fl[4] >> fl[5] >> fl[6] >> fl[7])
    for (auto i=0; i<8; ++i)
      graph[i] -> SetPoint(graph[i]->GetN(),x,fl[i]);

  /// draw
  auto hist = new TH1D("hist",";Energy [MeV/n];Flux [m^{-1}sr^{-1}s^{-1}]",400,1,100000);
  hist -> SetMaximum(1.5);
  hist -> SetMinimum(0.000001);

  auto lg = new TLegend();
  lg -> SetMargin(0.3);

  auto cvs = cc2();
  hist -> Draw();
  for (auto i=0; i<8; ++i) {
    graph[i] -> Draw("samel");
    lg -> AddEntry(graph[i],Form("Z=%d",i+1),"lg");
  }
  make(lg) -> Draw();

  cvs -> SetLogx();
  cvs -> SetLogy();
}
