void primary_energy()
{
  auto graph = new TGraph();

  Double_t e, flux;
  std::ifstream input("h_flux.dat");
  while (input >> e >> flux)
    graph -> SetPoint(graph->GetN(),e,flux);

  auto cvs = new TCanvas();
  graph -> GetHistogram() -> SetTitle(";Particle energy (MeV);Flux (m^{-2}sr^{-1}s^{-1})");
  graph -> Draw();

  cvs -> SetLogy();
  cvs -> SetLogx();
}
