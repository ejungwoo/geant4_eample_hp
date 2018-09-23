//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// Authors: S. Guatelli , M. G. Pia, INFN Genova and F. Ambroglini INFN Perugia, Italy
// 
//
#include "G4HumanPhantomPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

#include "G4RunManager.hh"
#include "G4ios.hh"

#include <fstream>
#include "TGraph.h"
#include "TF1.h"
#include "G4HumanPhantomAnalysisManager.hh"

G4HumanPhantomPrimaryGeneratorAction::G4HumanPhantomPrimaryGeneratorAction(G4HumanPhantomAnalysisManager *analysis) : analysisMan(analysis)
{
  particleGun = new G4ParticleGun(1);

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable -> FindParticle("proton");

  particleGun -> SetParticleDefinition(particle);
  particleGun -> SetParticleMomentumDirection(G4ThreeVector(0.,-1.,0.));
  particleGun -> SetParticlePosition(G4ThreeVector(-100.,500.,-300));

  graph = new TGraph();

  std::ifstream input("h_flux.dat");
  Double_t e, flux;
  while (input >> e >> flux)
    graph -> SetPoint(graph->GetN(),e,flux);

  fpdf = new TF1("flux_pdf",[&](double*x, double*){ return graph->Eval(x[0]); },1,10000,0);
}

G4HumanPhantomPrimaryGeneratorAction::~G4HumanPhantomPrimaryGeneratorAction()
{
  delete particleGun;
}

void G4HumanPhantomPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  particleGun -> SetParticleEnergy(fpdf->GetRandom()*MeV);
  particleGun -> GeneratePrimaryVertex(anEvent);

  G4double energy = particleGun -> GetParticleEnergy();
  analysisMan -> SetPrimaryEnergy(energy);
}

